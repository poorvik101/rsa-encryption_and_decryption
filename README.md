# 🔐 Secure Data Transmission over WiFi using RSA

![Platform](https://img.shields.io/badge/Platform-Arduino%20%7C%20ESP8266-blue)
![Encryption](https://img.shields.io/badge/Encryption-RSA-green)
![Communication](https://img.shields.io/badge/Communication-WiFi-orange)
![Subject](https://img.shields.io/badge/Subject-Communication%20Systems--1-lightgrey)
![Status](https://img.shields.io/badge/Status-Completed-brightgreen)

> **RSA-encrypted message transmission between two Arduino nodes over a WiFi network — implemented on low-cost microcontroller hardware.**

---

## 📸 Hardware Demo

| Transmitter Side | Receiver Side |
|:---:|:---:|
| Arduino Uno + Keypad + NodeMCU | Arduino Mega 2560 + LCD + NodeMCU |

*Transmitter: User types message via 4×4 keypad → RSA encrypted → sent over WiFi*  
*Receiver: Encrypted data received → private key entered → decrypted message shown on LCD*

---

## 🧠 System Overview

```
┌─────────────────────────────────────────────────────────────────┐
│                        TRANSMITTER SIDE                         │
│  [4x4 Keypad] ──► [Arduino Uno] ──RSA Encrypt──► [NodeMCU TX]  │
│                       (ATmega328P)    e=17, n=3233              │
└──────────────────────────────┬──────────────────────────────────┘
                               │  WiFi (ESP8266 → ESP8266)
                               ▼
┌─────────────────────────────────────────────────────────────────┐
│                         RECEIVER SIDE                           │
│  [NodeMCU RX] ──► [Arduino Mega 2560] ──RSA Decrypt──► [LCD]   │
│                        (ATmega2560)    d=2753, n=3233           │
└─────────────────────────────────────────────────────────────────┘
```

---

## ⚙️ Tech Stack

| Layer | Component |
|---|---|
| Microcontroller (TX) | Arduino Uno (ATmega328P) |
| Microcontroller (RX) | Arduino Mega 2560 (ATmega2560) |
| WiFi Module | NodeMCU ESP8266 (×2) |
| User Input | 4×4 Matrix Keypad |
| Display | 16×2 LCD with I2C module |
| Status Indicator | LED |
| Encryption | RSA (public-key: `e=17`, modulus: `n=3233`) |
| Decryption | RSA (private-key: `d=2753`, modulus: `n=3233`) |
| Communication Protocol | UART (Arduino ↔ NodeMCU), TCP/IP (NodeMCU ↔ NodeMCU) |

---

## 🔑 How RSA Works Here

1. **Key Generation**: Keys pre-generated — public key `(e=17, n=3233)`, private key `(d=2753, n=3233)`
2. **Encryption (TX)**: Each character `m` → ciphertext `c = m^e mod n`
3. **Transmission**: Ciphertext sent as space-separated integers over WiFi
4. **Decryption (RX)**: User enters private key → `m = c^d mod n` → original character recovered
5. **Validation**: Wrong private key → junk output on LCD

---

## 📁 Repository Structure

```
rsa-wifi-secure-transmission/
├── README.md
├── hardware/
│   ├── BOM.md                          ← Bill of Materials
│   └── circuit_description.md          ← Wiring guide
├── firmware/
│   ├── transmitter_arduino_uno/
│   │   └── transmitter_uno.ino         ← Encrypt + send via UART
│   ├── transmitter_nodemcu/
│   │   └── transmitter_esp8266.ino     ← Forward encrypted data over WiFi
│   ├── receiver_nodemcu/
│   │   └── receiver_esp8266.ino        ← Receive over WiFi, send to Mega
│   └── receiver_arduino_mega/
│       └── receiver_mega.ino           ← Decrypt + display on LCD
├── docs/
│   └── RSA_WiFi_Project_Report.pdf     ← Full project report
└── results/
    └── observations.md                 ← Test results and observations
```

---

## 🚀 How to Run

### Prerequisites
- Arduino IDE (v1.8+ or v2.x)
- ESP8266 board package installed in Arduino IDE
- Libraries: `LiquidCrystal_I2C`, `Keypad`, `ESP8266WiFi`

### Steps

**1. Configure WiFi credentials** in both NodeMCU sketches:
```cpp
const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";
```

**2. Set receiver IP** in `transmitter_esp8266.ino`:
```cpp
const char* receiverIP = "192.168.x.x"; // check receiver NodeMCU serial output
```

**3. Flash firmware in this order:**
```
1. receiver_esp8266.ino  → Receiver NodeMCU
2. receiver_mega.ino     → Arduino Mega 2560
3. transmitter_esp8266.ino → Transmitter NodeMCU
4. transmitter_uno.ino   → Arduino Uno
```

**4. Power up both setups** → LCD on transmitter shows `Enter Message:`

**5. Operation:**
- Type message on keypad → press `#` to encrypt and send
- Receiver LCD prompts `Enter Private Key:` → type `2753` → press `#`
- Original message displayed ✅

---

## 📊 Results

| Test | Input | Encrypted Output (sample) | Decrypted Output | Status |
|---|---|---|---|---|
| Single char `A` (ASCII 65) | `A` | `2790` | `A` | ✅ Pass |
| Word `HELLO` | `HELLO` | `2790 2345 1323 1323 1604` | `HELLO` | ✅ Pass |
| Wrong private key | any | ciphertext | junk/error | ✅ Correct behavior |

---

## ✅ Advantages

- End-to-end encryption over an insecure WiFi channel
- Public key can be shared openly; private key never transmitted
- LED + LCD feedback for transmission status
- Low-cost implementation under ₹1500 in components

## ⚠️ Known Limitations

- Small RSA key size (n=3233) used for demonstration — **not production-secure**
- RSA is slow for large messages; hybrid encryption (RSA + AES) recommended for real systems
- Vulnerable to quantum attacks (Shor's algorithm) — post-quantum alternatives like Kyber exist
- Exponentiation loop is naive — fast modular exponentiation should be used for larger keys

---

## 🔭 Future Improvements

- [ ] Implement hybrid RSA + AES encryption for bulk data
- [ ] Upgrade to 2048-bit RSA key using a more capable processor
- [ ] Add HMAC for message integrity verification
- [ ] Integrate TLS instead of raw TCP for the WiFi layer
- [ ] Migrate to ESP32 for better compute performance

---

## 👥 Team

| Name | USN |
|---|---|
| Poorvik K R | 02JST23UEC037 |
| Arhan Shariff | 02JST23UEC004 |
| Dhanush S | 02JST23UEC015 |

**Submitted to:** Dr. Gayitri H M, Associate Professor, Dept. of ECE  
**Institution:** SJCE, JSS Science and Technology University, Mysuru  
**Subject:** Communication Systems-1 (22EC350)

---

## 📚 References

- Raza et al. (2020) — Security Vulnerabilities in Wi-Fi Networks, *Int. Journal of Network Security*
- Sharma et al. (2019) — RSA and Its Application in Securing Wi-Fi Networks, *JISA*
- Goyal et al. (2021) — Optimization Techniques for RSA in IoT Devices, *ICC Proceedings*
- Singh & Gupta (2020) — Hybrid Cryptosystems for Secure Wi-Fi Communication

---

*Built as part of Communication Systems-1 coursework | JSS Science and Technology University*
