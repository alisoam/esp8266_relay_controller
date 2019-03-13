#include "peripherals.hpp"

#include "parameters.hpp"

RTC_DS1307 rtc;
WiFiClient espClient;

#define WIFI_SSID "Home 2"
#define WIFI_PASSWORD "74221776"

//#define WIFI_SSID "AndroidAP"
//#define WIFI_PASSWORD "swdm6003"


static void setupRtc()
{
  delay(100);
  if (! rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
}

static void setupWifi()
{
  delay(100);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.begin(getWifiSsid(), getWifiPassword());
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setupPeripherals()
{
  setupRtc();
  setupWifi();
}
