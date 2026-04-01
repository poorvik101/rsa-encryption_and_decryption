/*
 * RSA Secure Transmission - Receiver (Arduino Mega 2560)
 * ========================================================
 * Receives encrypted ciphertext from NodeMCU via UART.
 * Prompts user to enter the private key on a 4x4 keypad.
 * Decrypts using RSA and displays result on 16x2 LCD.
 *
 * RSA Parameters (demo-scale):
 *   Private key : d = 2753
 *   Modulus     : n = 3233  (p=61, q=53)
 *
 * Enter private key '2753' on keypad, then press '#'.
 * Wrong key → "Invalid Key!" shown on LCD.
 *
 * Hardware:
 *   - Arduino Mega 2560 (ATmega2560)
 *   - 4x4 Matrix Keypad (rows: 22,23,24,25 | cols: 26,27,28,29)
 *   - 16x2 LCD via I2C (address 0x27, SDA=20, SCL=21)
 *   - NodeMCU ESP8266 on Serial (RX pin 19 / Serial1)
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

// ── LCD Setup ──────────────────────────────────────────
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ── Keypad Setup ───────────────────────────────────────
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {22, 23, 24, 25};
byte colPins[COLS] = {26, 27, 28, 29};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// ── RSA Parameters ─────────────────────────────────────
const long n = 3233;   // Modulus
const long d = 2753;   // Private exponent

// ── RSA Decryption ─────────────────────────────────────
// m = c^d mod n  (naive exponentiation)
String decryptRSA(String encryptedMsg) {
  String decrypted = "";
  int i = 0;

  while (i < (int)encryptedMsg.length()) {
    // Parse space-separated integer tokens
    String num = "";
    while (i < (int)encryptedMsg.length() && encryptedMsg[i] != ' ') {
      num += encryptedMsg[i];
      i++;
    }
    i++;  // Skip space

    if (num.length() == 0) continue;

    long c = num.toInt();
    long decryptedChar = 1;
    for (int j = 0; j < d; j++) {
      decryptedChar = (decryptedChar * c) % n;
    }
    decrypted += (char)decryptedChar;
  }
  return decrypted;
}

// ── Read keypad input until '#' ────────────────────────
String getKeyInput() {
  String input = "";
  lcd.setCursor(0, 1);
  while (true) {
    char key = keypad.getKey();
    if (key) {
      if (key == '#') break;
      if (key == '*') {
        input = "";
        lcd.setCursor(0, 1);
        lcd.print("                ");  // Clear line
        lcd.setCursor(0, 1);
      } else {
        input += key;
        lcd.print('*');  // Mask input
      }
    }
  }
  return input;
}

// ── Setup ──────────────────────────────────────────────
void setup() {
  Serial.begin(9600);   // Debug
  Serial1.begin(9600);  // Communication with NodeMCU (pins 18/19)
  lcd.begin();
  lcd.backlight();
  lcd.print("Waiting for Msg.");
}

// ── Main Loop ──────────────────────────────────────────
void loop() {
  if (Serial1.available()) {
    String encryptedMessage = Serial1.readStringUntil('\n');
    encryptedMessage.trim();

    lcd.clear();
    lcd.print("Msg Received!");
    delay(1000);

    lcd.clear();
    lcd.print("Enter Priv Key:");
    String privateKey = getKeyInput();

    if (privateKey.toInt() == d) {
      String decryptedMessage = decryptRSA(encryptedMessage);
      lcd.clear();
      lcd.print("Decrypted:");
      lcd.setCursor(0, 1);
      lcd.print(decryptedMessage);
      delay(5000);
    } else {
      lcd.clear();
      lcd.print("Invalid Key!");
      delay(3000);
    }

    lcd.clear();
    lcd.print("Waiting for Msg.");
  }
}
