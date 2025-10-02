// ======================================================================
// LED_Programmierung.ino — Ausführlich kommentierte Lernversion (Deutsch)
// Themen, die erklärt werden:
//  • Was macht jede Variable und Funktion?
//  • Wie funktioniert Entprellung (debounce) mit millis()?
//  • Wie wird die Klickanzahl/Long‑Press erkannt (Zustandsautomat)?
//  • Wie steuert Adafruit_NeoPixel die LEDs (show/Color/HSV)?
//  • Sicherheitstipps (Strom trennen, GND gemeinsam).
// ======================================================================

// Bibliothek für adressierbare RGB‑LEDs (WS2812/NeoPixel). Stellt Klassen/Funktionen bereit, um Farben an einzelne LEDs zu senden.
#include <Adafruit_NeoPixel.h>

// Daten‑Pin des LED‑Streifens (DIN). Diesen Pin mit dem LED‑Eingang verbinden.
#define LED_PIN     8
// Anzahl der LEDs am Strip. Diese Zahl bestimmt Schleifen/Indizes in den Effekten.
#define PIXELCOUNT  8
// Pin des Tasters. Wird später mit digitalRead() abgefragt.
#define BUTTON_PIN  2

// NeoPixel‑Objekt anlegen: Parameter sind (Anzahl, Daten‑Pin, Farbreihenfolge+Protokoll).
// NEO_GRB + NEO_KHZ800 = Standard für viele WS2812B‑LEDs (800 kHz, Farbreihenfolge GRB).
Adafruit_NeoPixel strip(PIXELCOUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// Enumeration (Aufzählung) von Betriebsarten. Erleichtert das Umschalten der LED‑Modi.
enum Mode { FLAME, COLOR_SELECT, RAINBOW, RED_FLAME };
// Startmodus beim Einschalten. Kann per Klicks/Long‑Press umgeschaltet werden.
Mode currentMode = FLAME;

// Master‑Schalter: true = LEDs an; false = alle aus (Energiesparen / Pause).
bool ledsOn = true;
// Für Entprellung: speichert den vorigen stabilen Tasterzustand (HIGH/LOW).
// HIGH/LOW sind digitale Zustände: HIGH = 1 (5V), LOW = 0 (0V).
bool lastButtonState = HIGH;
// Direkt gelesener Tasterzustand. Mit Entprell‑Logik wird daraus ein stabiler Zustand.
bool currentButtonState;
// Zeit (in ms, von millis()), wann die letzte Zustandsänderung des Tasters erkannt wurde.
unsigned long lastDebounceTime = 0;
// Mindestzeit (ms), die ein neuer Tasterzustand stabil sein muss, bevor wir ihn akzeptieren.
const unsigned long debounceDelay = 50;

unsigned long lastClickTime = 0;
// Klickzähler: zählt kurze Tasterbetätigungen in einem Zeitfenster, um 1×, 2×, 3×, 4× zu unterscheiden.
int clickCount = 0;
unsigned long buttonPressStart = 0;

uint16_t hue = 0;
// Erstellt eine RGB‑Farbe. Reihenfolge hier passend zur NEO_GRB‑Konfiguration.
uint32_t selectedColor = strip.Color(255, 100, 0); // Startfarbe

// setup(): läuft genau einmal nach dem Einschalten/Reset. Initialisiert LEDs, Pins, Startwerte.
void setup() {
  // Pin‑Modus für Taster. Tipp: INPUT_PULLUP nutzt internen Widerstand; Taster dann gegen GND schalten.
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  // LED‑Objekt initialisieren. Muss vor setPixelColor()/show() aufgerufen werden.
  strip.begin();
  // Gesamthelligkeit (0–255) für alle LEDs (softwareseitig). Verringert Stromverbrauch/Blendung.
  strip.setBrightness(255);
  // Sendet die aktuellen Pixel‑Daten an den Strip. Ohne show() sieht man keine Änderung.
  strip.show();
  randomSeed(analogRead(0));
}

// loop(): läuft unendlich oft. Liest Taster, aktualisiert Modus und rendert LED‑Effekte.
void loop() {
  // Zugriff auf einen Pin. Bei Tastern: digitalRead(BUTTON_PIN) gibt HIGH/LOW zurück.
  currentButtonState = digitalRead(BUTTON_PIN);

  // Entprellung
  // if‑Bedingung: führt den Block nur aus, wenn die Bedingung wahr ist.
  if (currentButtonState != lastButtonState) {
    // millis(): Zeit seit Start in Millisekunden. Nützlich für Entprellung/Animationen ohne delay().
    lastDebounceTime = millis();
  }

  // Button gedrückt
  // HIGH/LOW sind digitale Zustände: HIGH = 1 (5V), LOW = 0 (0V).
  // if‑Bedingung: führt den Block nur aus, wenn die Bedingung wahr ist.
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    // millis(): Zeit seit Start in Millisekunden. Nützlich für Entprellung/Animationen ohne delay().
    buttonPressStart = millis();
  }

  // Button losgelassen
  // HIGH/LOW sind digitale Zustände: HIGH = 1 (5V), LOW = 0 (0V).
  // if‑Bedingung: führt den Block nur aus, wenn die Bedingung wahr ist.
  if (lastButtonState == LOW && currentButtonState == HIGH) {
    // millis(): Zeit seit Start in Millisekunden. Nützlich für Entprellung/Animationen ohne delay().
    unsigned long pressDuration = millis() - buttonPressStart;

    // if‑Bedingung: führt den Block nur aus, wenn die Bedingung wahr ist.
    if (pressDuration >= 600) {
      // Langes Drücken → Farbwahl beenden
      currentMode = FLAME;
    } else {
      // Kurzer Klick
      // millis(): Zeit seit Start in Millisekunden. Nützlich für Entprellung/Animationen ohne delay().
      unsigned long now = millis();
      // if‑Bedingung: führt den Block nur aus, wenn die Bedingung wahr ist.
      if (now - lastClickTime < 500) {
        // Klickzähler: zählt kurze Tasterbetätigungen in einem Zeitfenster, um 1×, 2×, 3×, 4× zu unterscheiden.
        clickCount++;
      } else {
        // Klickzähler: zählt kurze Tasterbetätigungen in einem Zeitfenster, um 1×, 2×, 3×, 4× zu unterscheiden.
        clickCount = 1;
      }
      lastClickTime = now;
    }
  }

  // Klick-Auswertung nach Timeout
  // millis(): Zeit seit Start in Millisekunden. Nützlich für Entprellung/Animationen ohne delay().
  // Klickzähler: zählt kurze Tasterbetätigungen in einem Zeitfenster, um 1×, 2×, 3×, 4× zu unterscheiden.
  // if‑Bedingung: führt den Block nur aus, wenn die Bedingung wahr ist.
  if ((millis() - lastClickTime) > 500 && clickCount > 0) {
    // Klickzähler: zählt kurze Tasterbetätigungen in einem Zeitfenster, um 1×, 2×, 3×, 4× zu unterscheiden.
    switch (clickCount) {
      // Auswertung einer Klickanzahl (1×, 2×, 3×, 4×) → Modus umschalten.
      case 1:
        ledsOn = !ledsOn;
        // Setzt alle Pixelpuffer auf Schwarz (0,0,0). Danach show() senden, damit es sichtbar wird.
        // Sendet die aktuellen Pixel‑Daten an den Strip. Ohne show() sieht man keine Änderung.
        // if‑Bedingung: führt den Block nur aus, wenn die Bedingung wahr ist.
        if (!ledsOn) { strip.clear(); strip.show(); }
        break;
      // Auswertung einer Klickanzahl (1×, 2×, 3×, 4×) → Modus umschalten.
      case 2:
        currentMode = RAINBOW;
        break;
      // Auswertung einer Klickanzahl (1×, 2×, 3×, 4×) → Modus umschalten.
      case 3:
        currentMode = FLAME;
        break;
      // Auswertung einer Klickanzahl (1×, 2×, 3×, 4×) → Modus umschalten.
      case 4:
        currentMode = RED_FLAME;
        break;
    }
    // Klickzähler: zählt kurze Tasterbetätigungen in einem Zeitfenster, um 1×, 2×, 3×, 4× zu unterscheiden.
    clickCount = 0;
  }

  // Farbwahl während langem Drücken
  // millis(): Zeit seit Start in Millisekunden. Nützlich für Entprellung/Animationen ohne delay().
  // HIGH/LOW sind digitale Zustände: HIGH = 1 (5V), LOW = 0 (0V).
  // if‑Bedingung: führt den Block nur aus, wenn die Bedingung wahr ist.
  if (currentButtonState == LOW && (millis() - buttonPressStart > 600)) {
    currentMode = COLOR_SELECT;
    hue += 200;
    if (hue > 65535) hue = 0;
    // HSV‑Farbraum: Farbton (Hue) über 0…65535; Sättigung/Helligkeit 0…255. Gut für Regenbogen/Übergänge.
    selectedColor = strip.gamma32(strip.ColorHSV(hue));
    // for‑Schleife: wiederholt Befehle mehrfach. Oft genutzt, um über alle LEDs zu iterieren.
    for (int i = 0; i < PIXELCOUNT; i++) {
      // Setzt die Farbe eines einzelnen Pixels (Index beginnt bei 0).
      strip.setPixelColor(i, selectedColor);
    }
    // Sendet die aktuellen Pixel‑Daten an den Strip. Ohne show() sieht man keine Änderung.
    strip.show();
    // delay(ms): einfache Pause. Achtung: blockiert loop(). Für reaktivere Steuerung besser millis() nutzen.
    delay(20);
  }

  lastButtonState = currentButtonState;

  // LED-Anzeige
  // if‑Bedingung: führt den Block nur aus, wenn die Bedingung wahr ist.
  if (ledsOn) {
    // Zustandsautomat: je nach currentMode unterschiedliche Effekte berechnen/anzeigen.
    switch (currentMode) {
      // FLAME: Simuliert flackerndes Feuer (zufällige Helligkeit/Farbton im warmen Bereich).
      case FLAME:
        {
          uint8_t r = (selectedColor >> 16) & 0xFF;
          uint8_t g = (selectedColor >> 8) & 0xFF;
          uint8_t b = selectedColor & 0xFF;
          // for‑Schleife: wiederholt Befehle mehrfach. Oft genutzt, um über alle LEDs zu iterieren.
          for (int i = 0; i < PIXELCOUNT; i++) {
            float f = random(50, 100) / 100.0;
            // Setzt die Farbe eines einzelnen Pixels (Index beginnt bei 0).
            strip.setPixelColor(i, r*f, g*f, b*f);
          }
          // Sendet die aktuellen Pixel‑Daten an den Strip. Ohne show() sieht man keine Änderung.
          strip.show();
          delay(random(30, 80));
        }
        break;

      // RED_FLAME: Wie FLAME, aber stärker im roten Bereich (dramatischer Feuereffekt).
      case RED_FLAME:
        {
          // for‑Schleife: wiederholt Befehle mehrfach. Oft genutzt, um über alle LEDs zu iterieren.
          for (int i = 0; i < PIXELCOUNT; i++) {
            float f = random(50, 100) / 100.0;
            // Setzt die Farbe eines einzelnen Pixels (Index beginnt bei 0).
            strip.setPixelColor(i, 255*f, 30*f, 0);
          }
          // Sendet die aktuellen Pixel‑Daten an den Strip. Ohne show() sieht man keine Änderung.
          strip.show();
          delay(random(30, 80));
        }
        break;

      // RAINBOW: uniforme Farbe wechselt automatisch über den HSV-Farbkreis
      case RAINBOW: {
        static uint16_t hueSolid = 0;       // eigener Hue, unabhängig vom globalen hue
        const uint16_t step = 1024;         // Schrittweite pro Frame (größer = schneller)

        hueSolid += step;
        if (hueSolid >= 65536) hueSolid -= 65536;

        uint32_t col = strip.gamma32(strip.ColorHSV(hueSolid));
        strip.fill(col, 0, PIXELCOUNT);     // alle LEDs auf dieselbe Farbe
        strip.show();
        delay(40);
        break;
      }

      // COLOR_SELECT: Farbe wird live während langem Drücken verändert (z. B. per Farbrad/HSV‑Wert).
      case COLOR_SELECT:
        // Wird oben gesteuert
        break;
    }
  } else {
    // Setzt alle Pixelpuffer auf Schwarz (0,0,0). Danach show() senden, damit es sichtbar wird.
    strip.clear();
    // Sendet die aktuellen Pixel‑Daten an den Strip. Ohne show() sieht man keine Änderung.
    strip.show();
  }
}
