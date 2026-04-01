# 📊 Observations & Test Results

## RSA Parameters Used
| Parameter | Value | Notes |
|-----------|-------|-------|
| p | 61 | Prime factor |
| q | 53 | Prime factor |
| n (modulus) | 3233 | p × q |
| φ(n) | 3120 | (p-1)(q-1) |
| e (public exp) | 17 | gcd(17, 3120) = 1 |
| d (private exp) | 2753 | e×d ≡ 1 (mod φ(n)) |

## Encryption Test Cases

| Input Char | ASCII (m) | Encrypted (c = m^e mod n) | Decrypted |
|-----------|-----------|--------------------------|-----------|
| A | 65 | 2790 | A ✅ |
| H | 72 | 3C7 (varies) | H ✅ |
| 1 | 49 | — | 1 ✅ |

## End-to-End Message Tests

| Message | Transmitted as | Received & Decrypted | Result |
|---------|---------------|----------------------|--------|
| `HI` | `2790 1814 ` | `HI` | ✅ Pass |
| `HELLO` | 5 space-sep integers | `HELLO` | ✅ Pass |
| `1234` | 4 integers | `1234` | ✅ Pass |
| `HI` with wrong key | — | `Invalid Key!` on LCD | ✅ Correct |

## Observations

- **Transmission latency**: ~1–2 seconds end-to-end over local WiFi
- **Key entry UX**: Private key input is masked with `*` on LCD for security
- **LCD truncation**: Ciphertext is longer than 16 chars; display shows first 16 chars only
- **Reliability**: Occasional dropped characters if message is long (>8 chars) — likely UART buffer issue
- **Decryption time**: Noticeable delay (~2–3 sec) for longer messages due to naive `O(d)` loop — fast modular exponentiation would fix this

## Known Issues

1. Naive `pow(m, e) mod n` loop is **O(e)** — for large keys this is unusable
2. `n = 3233` is a toy key — easily factored; use only for learning
3. No integrity check — a MITM can flip bits in ciphertext without detection
4. NodeMCU ↔ Arduino UART uses 5V/3.3V without level shifter in prototype (should be fixed in final build)
