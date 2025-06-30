# 📬 Briefkasten-Benachrichtigungs-System mit ESP8266 & Ultraschallsensor

Dieses Projekt erkennt automatisch, wenn ein Brief in den Briefkasten eingeworfen wird, und sendet eine E-Mail-Benachrichtigung an eine definierte Adresse.

Ein **HC-SR04 Ultraschallsensor** misst kontinuierlich den Abstand im Inneren des Briefkastens. Wenn sich der Abstand plötzlich stark ändert (z. B. durch einen neuen Brief), registriert ein **ESP8266 NodeMCU** dies als Ereignis und sendet über WLAN eine **Benachrichtigung per E-Mail** (SMTP).

Nach dem Versand wartet das System, bis der Abstand wieder stabil bleibt, um Mehrfachmeldungen zu vermeiden.

---

## 🧰 Verwendete Materialien

- **ESP8266 NodeMCU** – Mikrocontroller mit WLAN
- **HC-SR04 Ultraschallsensor** – zur Abstandsmessung
- **1 kΩ & 2 kΩ Widerstände** – für Spannungsteiler am ECHO-Pin (3.3V Pegelanpassung)
- **Breadboard** – zum Aufbau der Schaltung
- **Jumper-Kabel** – zur Verbindung aller Komponenten
- **Powerbank oder 5V Batterie** – Stromversorgung für den ESP8266
- **Micro-USB Kabel** – zur Stromversorgung und Programmierung

---


## ⚙️ Installation & Inbetriebnahme

1. **Arduino IDE installieren**  
   [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software)

2. **ESP8266 Board hinzufügen:**  
   In den Voreinstellungen der Arduino IDE folgenden Link eintragen:  
   `http://arduino.esp8266.com/stable/package_esp8266com_index.json`  
   Dann unter Boardverwalter „esp8266“ installieren.

3. **Benötigte Bibliotheken installieren:**
   - `ESP8266WiFi`
   - `ESP_Mail_Client`

## 📧 Gmail SMTP einrichten

Damit der ESP8266 über dein Gmail-Konto E-Mails verschicken kann, musst du ein App-spezifisches Passwort erstellen:

### Schritt-für-Schritt:

1. **Zwei-Faktor-Authentifizierung aktivieren:**  
   Gehe zu: [https://myaccount.google.com/security](https://myaccount.google.com/security)  
   Aktiviere die „Bestätigung in zwei Schritten“.

2. **App-Passwort erstellen:**  
   - Gehe auf: [https://myaccount.google.com/apppasswords](https://myaccount.google.com/apppasswords)  
   - Wähle: **App: Mail**, **Gerät: Eigenes (z. B. ESP8266)**  
   - Ein 16-stelliges Passwort wird angezeigt – **dieses verwenden!**

3. **Im Code verwenden:**  
   In `secrets.h` → `AUTHOR_PASSWORD` = dieses App-Passwort (kein normales Gmail-Passwort!)

### SMTP-Server-Einstellungen im Projekt:

- Host: `smtp.gmail.com`
- Port: `465` (SSL)

---

