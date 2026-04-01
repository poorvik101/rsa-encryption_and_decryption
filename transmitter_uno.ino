/*
 * RSA Secure Transmission - Transmitter (Arduino Uno)
 * =====================================================
 * Reads message from 4x4 keypad, encrypts using RSA,
 * sends encrypted ciphertext to NodeMCU via UART.
 *
 * RSA Parameters (demo-scale):
 *   Public key  : e = 17
 *   Modulus     : n = 3233  (p=61, q=53)
 *
 * Press '#' to encrypt and send.
 * Press '*' to clear current input.
 *
 * Hardware:
 *   - Arduino Uno (ATmega328P)
 *   - 4x4 Matrix Keypad (rows: 2,3,4,5 | cols: 6,7,8,9)
 *   - 16x2 LCD via I2C (address 0x27)
 *   - NodeMCU ESP8266 on Serial (TX pin 1 → NodeMCU RX)
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

byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 9};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// ── RSA Parameters ─────────────────────────────────────
const long n = 3233;  // Modulus (n = p * q = 61 * 53)
const long e = 17;    // Public exponent

// ── RSA Encryption ─────────────────────────────────────
// c = m^e mod n  (naive exponentiation — fine for small e)
String encryptRSA(String msg) {
  String encrypted = "";
  for (char c : msg) {
    long m = (long)c;
    long encryptedChar = 1;
    for (int i = 0; i < e; i++) {
      encryptedChar = (encryptedChar * m) % n;
    }
    encrypted += String(encryptedChar) + " ";
  }
  return encrypted;
}

// ── Setup ──────────────────────────────────────────────
void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  lcd.print("Enter Message:");
}

// ── Main Loop ──────────────────────────────────────────
void loop() {
  static String message = "";
  char key = keypad.getKey();

  if (key) {
    if (key == '#') {
      // Encrypt and transmit
      lcd.clear();
      lcd.print("Encrypting...");
      String encryptedMessage = encryptRSA(message);

      Serial.println(encryptedMessage);  // Send to NodeMCU

      lcd.clear();
      lcd.print("Sent:");
      lcd.setCursor(0, 1);
      // Show truncated (LCD is only 16 chars wide)
      lcd.print(encryptedMessage.substring(0, 16));

      delay(3000);
      message = "";
      lcd.clear();
      lcd.print("Enter Message:");

    } else if (key == '*') {
      // Clear input
      message = "";
      lcd.clear();
      lcd.print("Enter Message:");

    } else {
      // Append character
      message += key;
      lcd.setCursor(0, 1);
      lcd.print(message);
    }
  }
}
