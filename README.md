# BlastGateServo (Optimierte Version)

Dieses Projekt ist eine optimierte Version des ursprünglichen **Blast Gate Servo Controllers** für Arduino. 

### Optimierungen:
- **Nicht-blockierende Steuerung** mit `millis()` statt `delay()` → Reaktionsschneller & effizienter
- **Bessere Button-Verarbeitung** mit Entprellung (Debounce)
- **Strukturierte und erweiterbare Servo-Verwaltung**
- **Optimierte AC-Sensorsteuerung** → weniger Störungen, bessere Erkennung
- **Saubere Code-Struktur und Debugging-Optionen**

### Installationsanleitung:
1. Lade den Code auf einen Arduino hoch.
2. Stelle sicher, dass die Servos, Sensoren und LEDs korrekt angeschlossen sind.
3. Die Blast Gates öffnen sich automatisch, wenn ein Sensor einen Stromfluss erkennt oder per Button manuell geschaltet wird.

### Abhängigkeiten:
- `Servo.h`
- Arduino IDE

### Credits:
Der ursprüngliche Code wurde von **Greg Pringle** erstellt und am **2019-01-20** veröffentlicht. 
Diese optimierte Version wurde verbessert, um bessere Performance und Wartbarkeit zu ermöglichen.

Mehr Infos zu den 3D-Druck-Teilen: [Thingiverse Link](https://www.thingiverse.com/thing:3301904)
