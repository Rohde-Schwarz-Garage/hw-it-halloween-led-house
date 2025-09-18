#include <Adafruit_NeoPixel.h>

#define LED_PIN     8
#define PIXELCOUNT  30
#define BUTTON_PIN  2

Adafruit_NeoPixel strip(PIXELCOUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

enum Mode { FLAME, COLOR_SELECT, RAINBOW, RED_FLAME };
Mode currentMode = FLAME;

bool ledsOn = true;
bool lastButtonState = HIGH;
bool currentButtonState;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

unsigned long lastClickTime = 0;
int clickCount = 0;
unsigned long buttonPressStart = 0;

uint16_t hue = 0;
uint32_t selectedColor = strip.Color(255, 100, 0); // Startfarbe

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  strip.begin();
  strip.setBrightness(255);
  strip.show();
  randomSeed(analogRead(0));
}

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
    switch (clickCount) {
      case 1:
        ledsOn = !ledsOn;
        if (!ledsOn) { strip.clear(); strip.show(); }
        break;
      case 2:
        currentMode = RAINBOW;
        break;
      case 3:
        currentMode = FLAME;
        break;
      case 4:
        currentMode = RED_FLAME;
        break;
    }
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
    strip.show();
    delay(20);
  }

  lastButtonState = currentButtonState;

  // LED-Anzeige
  if (ledsOn) {
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
          strip.show();
          delay(random(30, 80));
        }
        break;

      case RED_FLAME:
        {
          for (int i = 0; i < PIXELCOUNT; i++) {
            float f = random(50, 100) / 100.0;
            strip.setPixelColor(i, 255*f, 30*f, 0);
          }
          strip.show();
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
        strip.show();
        delay(30);
        break;

      case COLOR_SELECT:
        // Wird oben gesteuert
        break;
    }
  } else {
    strip.clear();
    strip.show();
  }
}
