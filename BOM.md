# 🧾 Bill of Materials (BOM)

## Transmitter Side

| # | Component | Specification | Qty | Est. Cost (INR) |
|---|-----------|---------------|-----|-----------------|
| 1 | Arduino Uno | ATmega328P, 14 digital I/O | 1 | ₹450 |
| 2 | NodeMCU ESP8266 | WiFi module, 3.3V logic | 1 | ₹220 |
| 3 | 4×4 Matrix Keypad | Membrane type, 8-pin | 1 | ₹80 |
| 4 | LCD 16×2 with I2C | I2C address 0x27 | 1 | ₹150 |
| 5 | LED | 5mm, any color | 1 | ₹5 |
| 6 | Resistor | 220Ω (for LED) | 1 | ₹2 |
| 7 | Breadboard | Half-size | 1 | ₹60 |
| 8 | Jumper Wires | Male-to-male, Male-to-female | 1 set | ₹50 |

## Receiver Side

| # | Component | Specification | Qty | Est. Cost (INR) |
|---|-----------|---------------|-----|-----------------|
| 1 | Arduino Mega 2560 | ATmega2560, 54 digital I/O | 1 | ₹750 |
| 2 | NodeMCU ESP8266 | WiFi module, 3.3V logic | 1 | ₹220 |
| 3 | 4×4 Matrix Keypad | Membrane type, 8-pin | 1 | ₹80 |
| 4 | LCD 16×2 with I2C | I2C address 0x27 | 1 | ₹150 |
| 5 | Breadboard | Half-size | 1 | ₹60 |
| 6 | Jumper Wires | Male-to-male, Male-to-female | 1 set | ₹50 |

## **Total Estimated Cost: ~₹2,327**

---

## Wiring Notes

- Arduino Uno operates at 5V; NodeMCU at 3.3V — use a voltage divider on TX line from Uno to NodeMCU
- I2C LCD: SDA → A4 (Uno) / Pin 20 (Mega), SCL → A5 (Uno) / Pin 21 (Mega)
- Keypad rows → digital pins 2,3,4,5 (Uno) / 22,23,24,25 (Mega)
- Keypad cols → digital pins 6,7,8,9 (Uno) / 26,27,28,29 (Mega)
- NodeMCU powered from Arduino 3.3V pin or separate USB
