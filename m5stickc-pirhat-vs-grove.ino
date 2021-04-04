#include <M5StickC.h>
#include <Ambient.h>

#define PIR_HAT_PIN 33
#define PIR_GROVE_PIN 36
#define LED_PIN 10

#define LED_ON LOW
#define LED_OFF HIGH

WiFiClient client;
Ambient ambient;

const char *ssid = "";
const char *password = "";

unsigned int channelId = 0;
const char *writeKey = "";

void setup()
{
  M5.begin();
  Serial.begin(9600);

  M5.Axp.ScreenBreath(9);
  M5.Lcd.setRotation(3);
  M5.Lcd.setCursor(30, 30);
  M5.Lcd.setTextSize(2);

  pinMode(PIR_HAT_PIN, INPUT);
  pinMode(PIR_GROVE_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(LED_PIN, LED_ON);
    delay(250);
    digitalWrite(LED_PIN, LED_OFF);
    delay(250);
    Serial.print(".");
  }
  Serial.printf("WiFi connected\r\nIP address: %s\n", WiFi.localIP());

  ambient.begin(channelId, writeKey, &client);

  digitalWrite(LED_PIN, LED_ON);
  delay(3000);
  digitalWrite(LED_PIN, LED_OFF);
}

void loop()
{
  int countHat = 0;
  int countGrove = 0;
  for (int i = 0; i < 120; i++)
  {
    if (digitalRead(PIR_HAT_PIN)) countHat++;
    if (digitalRead(PIR_GROVE_PIN)) countGrove++;

    M5.Lcd.setCursor(10, 20);
    M5.Lcd.printf("HAT   : %3d\n", countHat);
    M5.Lcd.setCursor(10, 50);
    M5.Lcd.printf("Grove : %3d\n", countGrove);

    delay(500);
  }

  ambient.set(1, countHat);
  ambient.set(2, countGrove);
  ambient.send();
}