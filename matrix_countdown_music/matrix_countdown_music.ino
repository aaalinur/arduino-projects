/*
  ================================================================================
  Project: 8x8 LED Matrix Countdown with Music & Animation
  Author: Alinur
  Hardware: Arduino Uno/Nano, 1088AS 8x8 LED Matrix, Passive Buzzer
  
  Description:
  - 5 to 0 Countdown with synchronized buzzer beeps.
  - Glitch effect & silence phase transitions.
  - Custom MIDI melody playing from PROGMEM (Flash Memory).
  - Dynamic 8x8 matrix animation: Dancing Stickman & Pulsing Heart.
  ================================================================================
*/

#include <avr/pgmspace.h>

// Hardware Pin Configurations
const byte colPins[8] = {A3, A0, 13, 12, A1, 10, 11, A2};
const byte rowPins[8] = {9, 8, 7, 6, 5, 4, 3, 2};
const byte BUZZER_PIN = A4;

// Numbers 0-5 for Matrix Display (Stored in PROGMEM)
const byte numbers[6][8] PROGMEM = {
  {B00111100, B01000010, B01000110, B01001010, B01010010, B01100010, B01000010, B00111100},
  {B00010000, B00110000, B00010000, B00010000, B00010000, B00010000, B00010000, B00111100},
  {B00111100, B01000010, B00000010, B00000100, B00011000, B00100000, B01000000, B01111111},
  {B00111100, B01000010, B00000010, B00011100, B00000010, B00000010, B01000010, B00111100},
  {B00001000, B00011000, B00101000, B01001000, B01111111, B00001000, B00001000, B00001000},
  {B01111111, B01000000, B01111100, B00000010, B00000010, B00000010, B01000010, B00111100}
};

// Animation Frames: Dancing Stickman
const byte danceFrames[4][8] PROGMEM = {
  {B00011000, B00011000, B00011000, B01111110, B00011000, B00101000, B01000100, B10000010},
  {B00011000, B00011000, B00011000, B10011001, B00011000, B00010100, B00100010, B01000001},
  {B00011000, B00011000, B00011000, B00111100, B00011000, B00101000, B00010100, B00100010},
  {B00011000, B00011000, B00011000, B01011010, B00011000, B00101000, B01000100, B10000010}
};

// Animation Frames: Pulsing Heart
const byte heartFrames[3][8] PROGMEM = {
  {B00000000, B00100100, B01111110, B01111110, B00111100, B00011000, B00000000, B00000000},
  {B01100110, B11111111, B11111111, B11111111, B01111110, B00111100, B00011000, B00000000},
  {B01100110, B11111111, B11111111, B11111111, B11111111, B01111110, B00111100, B00011000}
};

// Audio Arrays (MIDI Notes, Rhythm, Sounding durations)
const byte melody[] PROGMEM = {
  69,70,77,70,77,77,75,73,72,75,73,72,70,77,70,77,77,75,73,72,75,73,72,70,
  70,82,80,77,78,80,75,74,77,75,73,72,75,73,72,70,72,72,72,73,75,69,70,77,
  70,77,77,75,73,72,75,73,72,70,77,70,77,77,75,73,72,75,73,72,70,70,82,80,
  77,78,80,75,74,77,75,73,72,75,73,72,70,72,72,72,73,75,77,75,77,75,77,70,
  77,75,77,75,77,70,77,75,77,75,77,80,77,75,77,75,77,75,77,70,77,75,77,80,
  78,77,75,75,77,75,77,68,70,73,77,75,73,72,69,70,72,73,70,77,75,80,77,78,
  80,82,78,77,75,75,84,82,80,78,77,75,74,77,80,82,83,82,80,78,70,72,73,75,
  69,70,77,70,77,77,75,73,72,75,73,72,70,77,70,77,77,75,73,72,75,73,72,70,
  70,82,80,77,78,80,75,74,77,75,73,72,75,73,72,70,72,72,72,73,75,77,75,77,
  75,77,70,77,75,77,75,77,70,77,75,77,75,77,80,77,75,77,75,77,75,77,70,77,
  75,77,80,78,77,75,75,77,75,77,68,70,73,77,75,77,75,77,77,80,82,83,82,77,
  75,77,75,77,68,70,73
};

const uint16_t rhythm[] PROGMEM = {
  367,366,933,367,1433,367,400,367,366,367,366,367,533,933,366,1433,366,367,366,367,366,367,366,567,
  533,567,1067,366,367,567,533,1667,366,367,366,334,366,367,366,367,366,367,366,366,933,367,366,900,
  366,1467,367,366,366,367,366,367,366,567,900,367,1467,366,366,367,366,367,366,367,533,533,567,1100,
  366,367,533,567,1634,366,367,366,366,367,366,367,366,367,366,367,366,900,733,567,533,567,533,367,
  366,567,533,533,567,367,366,533,567,533,567,366,367,2533,367,567,533,567,533,400,366,534,533,567,
  533,367,366,567,533,567,533,366,367,5300,167,200,167,2033,367,166,200,167,933,366,700,567,2567,
  367,366,733,200,366,1067,366,733,200,167,200,167,200,167,200,167,200,366,167,200,900,367,900,733,
  2200,367,366,900,366,1467,366,367,366,367,366,367,366,567,900,367,1466,367,366,367,366,367,366,
  367,533,567,533,1100,366,367,567,533,1633,367,366,367,366,367,366,367,366,367,366,367,366,900,
  733,567,533,567,533,400,367,533,567,533,533,367,366,567,534,567,533,366,367,2567,367,533,567,533,
  567,367,366,533,534,567,533,366,367,567,533,567,533,366,367,2567,366,567,533,567,900,366,533,567,
  533,933,367,533,567,533,534,366,366,1433
};

const uint16_t sounding[] PROGMEM = {
  333,333,866,300,866,333,366,300,300,300,300,300,300,866,300,866,333,333,333,333,333,333,333,333,
  500,500,500,333,333,500,500,500,300,300,300,300,333,333,333,333,333,333,333,333,333,333,333,866,
  333,866,333,333,333,333,333,333,333,333,866,333,866,300,300,300,300,300,300,300,300,500,500,500,
  300,300,500,500,500,333,333,333,333,333,333,333,333,333,333,333,333,866,700,500,500,500,500,333,
  333,500,500,500,533,333,300,500,500,500,500,300,300,1400,333,500,500,500,500,333,300,500,500,500,
  500,333,333,500,500,500,500,333,333,1400,166,200,166,1466,366,166,200,166,933,366,666,533,1033,
  333,333,700,133,300,866,333,700,200,166,200,166,200,166,200,166,200,366,166,200,666,366,866,700,
  700,333,333,866,333,866,333,333,333,333,333,333,333,333,866,333,866,333,333,333,333,333,333,333,
  333,533,500,500,333,333,500,500,500,333,333,333,333,333,333,333,333,333,333,333,333,866,700,500,
  500,500,500,333,300,500,500,500,500,333,333,500,500,500,500,333,333,1433,300,500,500,500,500,300,
  300,500,500,533,500,333,333,500,500,500,500,333,333,1400,333,500,500,500,500,333,500,500,500,500,
  366,500,500,500,500,333,333,1433
};

const uint16_t TOTAL_NOTES = sizeof(melody);

// State Machine Modes
enum Mode { COUNTDOWN, ZERO_SILENCE, GLITCH, MUSIC, FINISHED };

Mode mode = COUNTDOWN;
byte countdown = 5;
byte scanRow = 0;
uint16_t songNote = 0;

// Non-blocking Timing Variables
unsigned long modeStarted;
unsigned long nextScan;
unsigned long nextAlarm;
unsigned long nextNote;

// Convert MIDI note number to Frequency (Hz)
int midiToHz(byte note) {
  return (int)(440.0 * pow(2.0, (note - 69) / 12.0) + 0.5);
}

// Clear Matrix Pixels
void blankMatrix() {
  for (byte r = 0; r < 8; r++) {
    digitalWrite(rowPins[r], HIGH);
  }
  for (byte c = 0; c < 8; c++) {
    digitalWrite(colPins[c], LOW);
  }
}

// Draw a single row multiplexed
void drawRow(byte bits) {
  blankMatrix();
  for (byte c = 0; c < 8; c++) {
    digitalWrite(colPins[c], bitRead(bits, 7 - c));
  }
  digitalWrite(rowPins[scanRow], LOW);
  scanRow = (scanRow + 1) & 7;
}

// Handle Animation Frames during Music State
byte musicPixels(byte row) {
  // 2.4s dancing stickman, then 2.4s pulsing heart
  byte phase = (millis() / 2400) & 1;

  if (phase == 0) {
    byte frame = (millis() / 150) & 3;
    return pgm_read_byte(&danceFrames[frame][row]);
  }

  const byte beat[] = {0, 1, 2, 1, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0};
  byte frame = beat[(millis() / 110) % 14];

  return pgm_read_byte(&heartFrames[frame][row]);
}

// Non-blocking Matrix Refresh Rate
void refreshMatrix() {
  unsigned long now = micros();

  if ((long)(now - nextScan) < 0) return;

  nextScan = now + 1100;

  byte bits = 0;

  if (mode == COUNTDOWN || mode == ZERO_SILENCE) {
    bits = pgm_read_byte(&numbers[countdown][scanRow]);
  } else if (mode == GLITCH) {
    bits = ((micros() >> 4) * 37 + scanRow * 91) ^ ((micros() >> 9) * 13);
  } else if (mode == MUSIC) {
    bits = musicPixels(scanRow);
  }

  drawRow(bits);
}

// Start Music State
void startMusic(unsigned long now) {
  mode = MUSIC;
  songNote = 0;
  nextNote = now;
}

// Main State Machine Updater
void updateScenario() {
  unsigned long now = millis();

  if (mode == COUNTDOWN) {
    if ((long)(now - nextAlarm) >= 0) {
      byte pitch = 650 + (5 - countdown) * 150;
      tone(BUZZER_PIN, pitch, 100 + (5 - countdown) * 28);
      nextAlarm = now + 1000;
    }

    if (now - modeStarted >= 1000) {
      modeStarted += 1000;

      if (countdown == 0) {
        noTone(BUZZER_PIN);
        mode = ZERO_SILENCE;
        modeStarted = now;
      } else {
        countdown--;
      }
    }
  }
  else if (mode == ZERO_SILENCE) {
    if (now - modeStarted >= 160) {
      mode = GLITCH;
      modeStarted = now;
    }
  }
  else if (mode == GLITCH) {
    if (now - modeStarted >= 520) {
      startMusic(now);
    }
  }
  else if (mode == MUSIC) {
    if ((long)(now - nextNote) >= 0) {
      if (songNote >= TOTAL_NOTES) {
        noTone(BUZZER_PIN);
        mode = FINISHED;
        return;
      }

      uint16_t gap = pgm_read_word(&rhythm[songNote]);
      uint16_t len = pgm_read_word(&sounding[songNote]);

      tone(BUZZER_PIN, midiToHz(pgm_read_byte(&melody[songNote])), len);

      nextNote = now + gap;
      songNote++;
    }
  }
}

void setup() {
  for (byte i = 0; i < 8; i++) {
    pinMode(colPins[i], OUTPUT);
    pinMode(rowPins[i], OUTPUT);
  }

  pinMode(BUZZER_PIN, OUTPUT);
  blankMatrix();

  modeStarted = millis();
  nextAlarm = modeStarted;
  nextScan = micros();
}

void loop() {
  refreshMatrix();
  updateScenario();
}
