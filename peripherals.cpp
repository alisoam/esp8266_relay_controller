#include <Wire.h>
#include <RTClib.h>
#include <ESP8266WiFi.h>

RTC_DS1307 rtc;
WiFiClient espClient;

#define WIFI_SSID "Home 2"
#define WIFI_PASSWORD "74221776"

//#define WIFI_SSID "AndroidAP"
//#define WIFI_PASSWORD "swdm6003"


static void setupRtc()
{
  delay(10);
  if (! rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
}

static void setupWifi()
{
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void peripheralsSetup()
{
  Serial.begin(115200);
  Serial.println("start");
  Wire.begin();
  setupRtc();
  setupWifi();
}
