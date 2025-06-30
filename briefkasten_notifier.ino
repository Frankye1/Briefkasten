#include <ESP8266WiFi.h>
#include <ESP_Mail_Client.h>

// WLAN
#define WIFI_SSID "WLAN_NAME"
#define WIFI_PASSWORD "WLAN_PASSWORD"

// Gmail SMTP
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
#define AUTHOR_EMAIL "Author_Email"
#define AUTHOR_PASSWORD "App_Password"
#define RECIPIENT_EMAIL "receipe_Email"

// Sensor
#define TRIG_PIN D1
#define ECHO_PIN D2

SMTPSession smtp;

float lastDistance = 0.0;
bool mailSent = false;
unsigned long lastChangeTime = 0;
const float changeThreshold = 1.0;       
const unsigned long resetDelay = 10000;  

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("🔌 Verbinde mit WLAN");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WLAN verbunden!");

  delay(2000);
  lastDistance = measureDistance();
  Serial.print("📏 Startwert: ");
  Serial.print(lastDistance);
  Serial.println(" cm");
  lastChangeTime = millis();
}

void loop() {
  float current = measureDistance();
  float delta = abs(current - lastDistance);

  Serial.print("📐 Jetzt: ");
  Serial.print(current);
  Serial.print(" cm | Vorher: ");
  Serial.print(lastDistance);
  Serial.print(" cm | Änderung: ");
  Serial.print(delta);
  Serial.println(" cm");

  // Wenn plötzlich große Änderung erkannt und noch keine Mail geschickt
  if (delta >= changeThreshold && !mailSent) {
    Serial.println("📬 Änderung erkannt – Sende E-Mail!");
    sendMail("📬 Brief erkannt!", "Ein Brief wurde eingeworfen (schnelle Änderung von Abstand).");
    mailSent = true;
    lastChangeTime = millis(); // Zeit merken
  }

  // Wenn wieder stabil für eine Weile → Reset erlauben
  if (mailSent && delta < 0.5) {
    if (millis() - lastChangeTime > resetDelay) {
      Serial.println("🔄 Keine Änderung seit 10 Sekunden – Zurücksetzen erlaubt");
      mailSent = false;
    }
  } else {
    lastChangeTime = millis(); // Bewegung erkannt → Timer zurücksetzen
  }

  lastDistance = current;
  delay(500); // alle 0.5 Sekunden messen
}

float measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // max 30 ms
  return duration * 0.0343 / 2.0;
}

void sendMail(String subject, String body) {
  SMTP_Message message;
  message.sender.name = "ESP8266 Briefkasten";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = subject;
  message.addRecipient("Empfänger", RECIPIENT_EMAIL);
  message.text.content = body;
  message.text.charSet = "utf-8";
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

  ESP_Mail_Session session;
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;

  if (!smtp.connect(&session)) {
    Serial.println("❌ SMTP-Verbindung fehlgeschlagen");
    return;
  }

  if (!MailClient.sendMail(&smtp, &message)) {
    Serial.println("❌ Fehler beim E-Mail-Versand:");
    Serial.println(smtp.errorReason());
  } else {
    Serial.println("✅ E-Mail wurde erfolgreich gesendet!");
  }

  smtp.closeSession();
}
