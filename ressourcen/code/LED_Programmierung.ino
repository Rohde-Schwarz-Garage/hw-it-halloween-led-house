// =============================================================
// LED_Programmierung.ino — Erklärte Version
// =============================================================

// Bibliothek für adressierbare RGB-LEDs (WS2812/NeoPixel) einbinden
#include <Adafruit_NeoPixel.h>

// An welchem Mikrocontroller-Pin das Datenkabel der LED-Strips hängt
#define LED_PIN     8
// Wie viele LEDs hat dein Strip/Deine Kette insgesamt?
#define PIXELCOUNT  30
// An diesem Pin ist der Taster angeschlossen
#define BUTTON_PIN  2

// NeoPixel-Objekt erstellen: Anzahl LEDs, Daten-Pin und Protokoll (GRB, 800 kHz)
Adafruit_NeoPixel strip(PIXELCOUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// Verschiedene Anzeigemodi der LEDs (Zustandsautomat)
enum Mode { FLAME, COLOR_SELECT, RAINBOW, RED_FLAME };
// Aktueller Modus beim Start: FLAME
Mode currentMode = FLAME;

// Merker: Sollen die LEDs überhaupt leuchten? (Ein/Aus)
bool ledsOn = true;
// Letzter bekannter Tasterzustand für Entprell-Logik
bool lastButtonState = HIGH;
// Aktuell gelesener Tasterzustand
bool currentButtonState;
// Zeitstempel, wann der Taster zuletzt stabil wurde (für Entprellung)
unsigned long lastDebounceTime = 0;
// Entprell-Zeit in Millisekunden: kurze Störungen am Taster werden ignoriert
const unsigned long debounceDelay = 50;

unsigned long lastClickTime = 0;
int clickCount = 0;
unsigned long buttonPressStart = 0;

uint16_t hue = 0;
uint32_t selectedColor = strip.Color(255, 100, 0); // Startfarbe

// setup(): Läuft genau einmal beim Start. Pins/Bibliotheken initialisieren.
void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  strip.begin();
  strip.setBrightness(255);
// Neue Pixel‑Daten an den LED‑Streifen senden
  strip.show();
  randomSeed(analogRead(0));
}

// loop(): Läuft immer wieder. Taster lesen, Modus wählen, LEDs updaten.
void loop() {
  currentButtonState = digitalRead(BUTTON_PIN);

  // Entprellung
  if (currentButtonState != lastButtonState) {
    lastDebounceTime = millis();
  }

  // Button gedrückt
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    buttonPressStart = millis();
  }

  // Button losgelassen
  if (lastButtonState == LOW && currentButtonState == HIGH) {
    unsigned long pressDuration = millis() - buttonPressStart;

    if (pressDuration >= 600) {
      // Langes Drücken → Farbwahl beenden
      currentMode = FLAME;
    } else {
      // Kurzer Klick
      unsigned long now = millis();
      if (now - lastClickTime < 500) {
        clickCount++;
      } else {
        clickCount = 1;
      }
      lastClickTime = now;
    }
  }

  // Klick-Auswertung nach Timeout
  if ((millis() - lastClickTime) > 500 && clickCount > 0) {
// Je nach Klickanzahl/Tasteraktion anderen Modus wählen
    switch (clickCount) {
// 1× drücken: LEDs An/Aus umschalten
      case 1:
        ledsOn = !ledsOn;
        if (!ledsOn) { strip.clear(); strip.show(); }
        break;
// 2× drücken: Regenbogen-Modus aktivieren
      case 2:
        currentMode = RAINBOW;
        break;
// 3× drücken: Flammen-/Feuer‑Modus aktivieren
      case 3:
        currentMode = FLAME;
        break;
// 4× drücken: Roter Flammen‑Modus aktivieren
      case 4:
        currentMode = RED_FLAME;
        break;
    }
// Klickzähler zurücksetzen, damit neue Sequenzen erkannt werden
    clickCount = 0;
  }

  // Farbwahl während langem Drücken
  if (currentButtonState == LOW && (millis() - buttonPressStart > 600)) {
    currentMode = COLOR_SELECT;
    hue += 200;
    if (hue > 65535) hue = 0;
    selectedColor = strip.gamma32(strip.ColorHSV(hue));
    for (int i = 0; i < PIXELCOUNT; i++) {
      strip.setPixelColor(i, selectedColor);
    }
// Neue Pixel‑Daten an den LED‑Streifen senden
    strip.show();
// Kurze Pause, um Effekte sichtbar zu machen / Timing zu steuern
    delay(20);
  }

  lastButtonState = currentButtonState;

  // LED-Anzeige
  if (ledsOn) {
// Je nach Klickanzahl/Tasteraktion anderen Modus wählen
    switch (currentMode) {
      case FLAME:
        {
          uint8_t r = (selectedColor >> 16) & 0xFF;
          uint8_t g = (selectedColor >> 8) & 0xFF;
          uint8_t b = selectedColor & 0xFF;
          for (int i = 0; i < PIXELCOUNT; i++) {
            float f = random(50, 100) / 100.0;
            strip.setPixelColor(i, r*f, g*f, b*f);
          }
// Neue Pixel‑Daten an den LED‑Streifen senden
          strip.show();
// Kurze Pause, um Effekte sichtbar zu machen / Timing zu steuern
          delay(random(30, 80));
        }
        break;

      case RED_FLAME:
        {
          for (int i = 0; i < PIXELCOUNT; i++) {
            float f = random(50, 100) / 100.0;
            strip.setPixelColor(i, 255*f, 30*f, 0);
          }
// Neue Pixel‑Daten an den LED‑Streifen senden
          strip.show();
// Kurze Pause, um Effekte sichtbar zu machen / Timing zu steuern
          delay(random(30, 80));
        }
        break;

      case RAINBOW:
        hue += 100;
        if (hue > 65535) hue = 0;
        for (int i = 0; i < PIXELCOUNT; i++) {
          uint16_t pixelHue = hue + (i * 65536L / PIXELCOUNT);
          strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
        }
// Neue Pixel‑Daten an den LED‑Streifen senden
        strip.show();
// Kurze Pause, um Effekte sichtbar zu machen / Timing zu steuern
        delay(30);
        break;

// Farbwahl-Modus: Farbe während langem Drücken auswählen (wird oben gesteuert)
      case COLOR_SELECT:
        // Wird oben gesteuert
        break;
    }
  } else {
// Alle Pixel auf 0 setzen (schwarz/aus)
    strip.clear();
// Neue Pixel‑Daten an den LED‑Streifen senden
    strip.show();
  }
}
