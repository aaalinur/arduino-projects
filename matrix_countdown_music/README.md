# ⏳ 8x8 LED Matrix Countdown with Music & Animation

An interactive Arduino project featuring a **5-to-0 countdown**, smooth transitions with glitch effects, custom **MIDI melody playback** using PROGMEM, and **dynamic 8x8 LED matrix animations** (Dancing Stickman & Pulsing Heart).

---

## 📌 Features

- **Dynamic Countdown:** Visual 5 to 0 numbers with pitch-shifting buzzer keep-beeps.
- **Glitch Effect Transition:** Pseudo-random pixel glitch transition before music starts.
- **PROGMEM Optimization:** Melody notes and rhythm arrays stored directly in Flash memory to save RAM.
- **Multi-Frame Animations:** Non-blocking 8x8 LED display switching between a dancing stickman and a pulsing heart beat.
- **Non-blocking Architecture:** Precise timing using `micros()` and `millis()` instead of `delay()`.

---

## 🛠️ Hardware Requirements

| Component | Quantity | Notes |
| :--- | :---: | :--- |
| **Arduino Uno / Nano** | 1 | Microcontroller board |
| **1088AS 8x8 LED Matrix** | 1 | Single color, cathode/anode row-col configuration |
| **Passive Buzzer** | 1 | Connected to Analog / Digital pin |
| **Resistors** | 8 | 220Ω (Recommended for LED columns/rows) |
| **Breadboard & Wires** | — | Jumper wires for connections |

---

## 🔌 Pin Configuration

### 8x8 LED Matrix (1088AS)
- **Column Pins:** `A3, A0, 13, 12, A1, 10, 11, A2`
- **Row Pins:** `9, 8, 7, 6, 5, 4, 3, 2`

### Passive Buzzer
- **Positive Pin (+):** `A4`
- **Negative Pin (-):** `GND`

---

## 🚀 How to Use

1. Clone or download this repository.
2. Open `matrix_countdown_music.ino` in the Arduino IDE.
3. Select your Board (*Arduino Uno*) and Port under **Tools**.
4. Click **Upload** to flash the microcontroller.
