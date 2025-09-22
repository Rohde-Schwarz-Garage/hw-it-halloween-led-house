
# Halloweenhaus – Bauanleitung für Zuhause

## Projektbeschreibung

Das Halloweenhaus ist ein DIY-Projekt für Kinder und Eltern, das Basteln, Technik und Kreativität verbindet. Mit einem Lasercutter, einfachen Elektronik-Komponenten (Arduino, NeoPixel-LED, Button) und etwas Heißkleber entsteht ein leuchtendes, individuell gestaltbares Halloweenhaus. Die Anleitung ist so geschrieben, dass sie auch ohne Vorkenntnisse leicht verständlich ist und Schritt für Schritt durch den gesamten Bauprozess führt.

---

## Was erwartet dich?

- **Lasercutter-Dateien** für alle Holz- und Kunststoffteile
- **Einfache Verkabelung** mit Arduino Uno R3 und NeoPixel-LED
- **Programmier-Anleitung** für verschiedene LED-Modi (Kerzenlicht, Farbwechsel, Regenbogen)
- **Bebilderte Schritt-für-Schritt-Anleitung** für den Zusammenbau
- **Tipps für kindgerechtes Basteln und sicheres Arbeiten**

---

## Zielgruppe

- Kinder (mit Unterstützung von Erwachsenen)
- Eltern, die gemeinsam mit ihren Kindern basteln möchten
- Einsteiger in Arduino- und Lasercutter-Projekte

---

## Was brauchst du?

- Zugang zu einem Lasercutter (z. B. in der Schule, im Makerspace oder FabLab)
- Arduino Uno R3 (oder kompatibel)
- NeoPixel-LED (WS2812)
- Heißkleber, Holzleim, ggf. Schleifpapier
- Button (Taster)
- Plexiglas für die Fenster
- Die bereitgestellten LightBurn-Dateien und Arduino-Code

---

## Bauanleitung (Kurzüberblick)

### 1. Laserdateien vorbereiten

Öffne das Programm **LightBurn** zweimal. Im ersten Fenster öffnest du die Datei **„Kuerbis_Haus_Bausteine.lbrn2“** und im zweiten Fenster die Datei **„Holzbauteile.lbrn2“**.

In der Datei **„Bausteine“** benötigst du folgende Teile: **Zubehör, Deckel, Boden, Vorderseite, Rückseite sowie die linke und rechte Seite.**

Lege die Teile platzsparend an und bestimme die Reihenfolge der Laserarbeiten:
- Schwarz (00)
- Grün (03)
- Türkis (06)
- Blau (01)
- Rot (02)

Speichere die Datei unter einem passenden Namen. Zusammen mit der Datei **„Kunststoffbauteile“** hast du nun alle Bauteile für dein Halloweenhaus.

Bevor du mit dem Lasern beginnst, schalte die Maschine ein und prüfe die wichtigsten Einstellungen (Position, Maschine, Leistung). Sende die Datei an den Lasercutter und folge den Anweisungen zum Einlegen und Ausrichten des Materials.

### 2. Chip vorbereiten und programmieren

Installiere die Arduino IDE und die Bibliothek **Adafruit NeoPixel** (Version 1.15.1). Verbinde den Chip mit dem Computer und lade das Programm **LED_Programmierung.ino** hoch. Die LED-Modi werden über den Button gesteuert:
- 1x drücken = An/Aus
- 2x drücken = Farbverlauf
- 3x drücken = Buntes Feuer
- 4x drücken = Kerzenlicht
- Gedrückt halten = Farbauswahl

### 3. Verkabeln

- Button: Eine Seite an GND, andere an D2
- LED: Data-In an D8, +5V an 5V, GND an GND
- Steckleitungen männlich/weiblich verwenden

### 4. Haus zusammenkleben

Klebe die Teile mit Heißkleber zusammen (Achtung: heiß!). Beginne mit Rückwand und Seitenwänden, dann Chip-Halter, Chip, LED, Front und Fenster. Fixiere alles sauber und prüfe die Passform vor dem endgültigen Verkleben.

---

## Viel Spaß beim Basteln und ein schaurig-schönes Halloween!
