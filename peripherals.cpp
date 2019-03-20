#include "peripherals.hpp"

#include "parameters.hpp"

RTC_DS1307 rtc;
WiFiClient espClient;

IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,22);
IPAddress subnet(255,255,255,0);

static void startAp()
{
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP(getApSsid(), getApPassword()) ? "Ready" : "Failed!");
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
}

static void startWifi()
{
  Serial.print("Connecting to ");
  Serial.println(getWifiSsid());
  WiFi.begin(getWifiSsid(), getWifiPassword());
}

static void setupRtc()
{
  delay(100);
  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (!rtc.isrunning())
  {
    Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

static void setupWifi()
{
  Serial.println();

  delay(100);
  startWifi();

  delay(100);
  startAp();

  WiFi.hostname(getDeviceName());
}

void restartWifi()
{
  WiFi.disconnect();
  startWifi();
}


void restartAp()
{
  WiFi.softAPdisconnect();
  startAp();
}

void setupPeripherals()
{
  setupRtc();
  setupWifi();
}
