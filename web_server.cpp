#include "web_server.hpp"

#include <time.h>

#include <FS.h>
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>

#include "mqtt.hpp"
#include "switches.hpp"
#include "parameters.hpp"
#include "peripherals.hpp"

#define JSON_BUFFER_SIZE    300

static ESP8266WebServer webServer(80);
StaticJsonDocument<500> jsonDoc;


char jsonBuffer[JSON_BUFFER_SIZE + 1];

static void handleNotFound()
{
  File file = SPIFFS.open("/web/404.html.gz", "r");
  webServer.streamFile(file, "text/html");
}

static void root()
{
  File file = SPIFFS.open("/web/index.html.gz", "r");
  webServer.streamFile(file, "text/html");
}

static void about()
{
  File file = SPIFFS.open("/web/about.html.gz", "r");
  webServer.streamFile(file, "text/html");
}

static void settings()
{
  File file = SPIFFS.open("/web/settings.html.gz", "r");
  webServer.streamFile(file, "text/html");
}

static void settingsDevice()
{
  File file = SPIFFS.open("/web/device.html.gz", "r");
  webServer.streamFile(file, "text/html");
}

static void settingsWifi()
{
  File file = SPIFFS.open("/web/wifi.html.gz", "r");
  webServer.streamFile(file, "text/html");
}

static void settingsAp()
{
  File file = SPIFFS.open("/web/ap.html.gz", "r");
  webServer.streamFile(file, "text/html");
}

static void settingsMqtt()
{
  File file = SPIFFS.open("/web/mqtt.html.gz", "r");
  webServer.streamFile(file, "text/html");
}

static void apiDeviceGet()
{
  jsonDoc.clear();
  jsonDoc["name"] = getDeviceName();
  DateTime t = rtc.now();
  snprintf(jsonBuffer, JSON_BUFFER_SIZE, "%u-%u-%u %u:%u:%u", (unsigned int)(t.year()), (unsigned int)(t.month()), (unsigned int)(t.day()),
                                                              (unsigned int)(t.hour()), (unsigned int)(t.minute()), (unsigned int)(t.second()));
  jsonDoc["time"] = jsonBuffer;
  serializeJson(jsonDoc, jsonBuffer, JSON_BUFFER_SIZE);
  webServer.send(200, "application/json", jsonBuffer);
}

static void apiDevicePost()
{
  jsonDoc.clear();
  deserializeJson(jsonDoc, webServer.arg("plain"));
  
  if ((jsonDoc["time"].isNull() or jsonDoc["time"].isNull()) and jsonDoc["name"].isNull())
  {
    webServer.send(400, "application/json", R"_({"message":"name or (date and time) should be provided"})_");
    return;
  }
  if (!jsonDoc["date"].isNull() and !jsonDoc["time"].isNull())
  {
    struct tm tm;
    strptime(jsonDoc["date"], "%Y-%m-%d", &tm);
    strptime(jsonDoc["time"], "%H:%M", &tm);
    rtc.adjust(DateTime(tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, 0));
  }
  if (!jsonDoc["name"].isNull())
  {
    setDeviceName(jsonDoc["name"]);
  }
  webServer.send(200, "application/json", "{}");
}

static void apiWifiGet()
{
  jsonDoc.clear();
  jsonDoc["active"] = getWifiActive();
  jsonDoc["connected"] = WiFi.isConnected();
  jsonDoc["ssid"] = getWifiSsid();
  serializeJson(jsonDoc, jsonBuffer, JSON_BUFFER_SIZE);
  webServer.send(200, "application/json", jsonBuffer);
}

static void apiWifiPost()
{
  jsonDoc.clear();
  String msg = webServer.arg("plain");
  deserializeJson(jsonDoc, msg);
  Serial.println(msg);
  if (jsonDoc["active"].isNull() or jsonDoc["ssid"].isNull())
  {
    webServer.send(400, "application/json", R"_({"message":"ssid and active should be provided"})_");
    return;
  }
  setWifiActive(jsonDoc["active"], false);
  setWifiSsid(jsonDoc["ssid"], false);
  if (!jsonDoc["password"].isNull() and strlen(jsonDoc["password"].as<const char*>()) > 0)
    setWifiPassword(jsonDoc["password"], false);
  restartWifi();
  webServer.send(200, "application/json", "{}");
}

static void apiApGet()
{
  jsonDoc.clear();
  jsonDoc["active"] = getApActive();
  jsonDoc["ssid"] = getApSsid();
  serializeJson(jsonDoc, jsonBuffer, JSON_BUFFER_SIZE);
  webServer.send(200, "application/json", jsonBuffer);
}

static void apiApPost()
{
  jsonDoc.clear();
  deserializeJson(jsonDoc, webServer.arg("plain"));
  if (jsonDoc["active"].isNull() or jsonDoc["ssid"].isNull())
  {
    webServer.send(400, "application/json", R"_({"message":"ssid and active should be provided"})_");
    return;
  }
  setApActive(jsonDoc["active"], false);
  setApSsid(jsonDoc["ssid"], false);
  if (jsonDoc["password"] != "")
  {
    if (strlen(jsonDoc["password"]) <= 7)
    {
      webServer.send(400, "application/json", R"_({"message":"password should be longer than 7 characters."})_");
      return;
    }
    setApPassword(jsonDoc["password"], false);
  }
  restartAp();
  webServer.send(200, "application/json", "{}");
}

static void apiMqttGet()
{
  jsonDoc.clear();
  jsonDoc["active"] = getMqttActive();
  jsonDoc["secure"] = false;
  jsonDoc["connected"] = mqttClient.connected();
  jsonDoc["hostname"] = getMqttHostname();
  jsonDoc["port"] = getMqttPort();
  jsonDoc["username"] = getMqttUsername();
  serializeJson(jsonDoc, jsonBuffer, JSON_BUFFER_SIZE);
  webServer.send(200, "application/json", jsonBuffer);
}

static void apiMqttPost()
{
 jsonDoc.clear();
  deserializeJson(jsonDoc, webServer.arg("plain"));
  if (jsonDoc["active"].isNull() or jsonDoc["secure"].isNull() or jsonDoc["hostname"].isNull() or 
      jsonDoc["port"].isNull() or jsonDoc["username"].isNull())
  {
    webServer.send(400, "application/json", R"_({"message":"active, secure, hostname, port and username should be provided"})_");
    return;
  }
  setMqttActive(jsonDoc["active"], false);
  setMqttHostname(jsonDoc["hostname"], false);
  setMqttPort(jsonDoc["port"], false);
  setMqttUsername(jsonDoc["username"], false);
  if (strlen(jsonDoc["password"]) > 0)
    setMqttPassword(jsonDoc["password"], false);
  restartMqtt();
  webServer.send(200, "application/json", "{}");
}

static void apiSwitchGet()
{
  jsonDoc.clear();
  auto states = jsonDoc.createNestedArray("states");
  for (unsigned int i = 0; i < SWITCHES_NUMBER; i++)
    states.add(switchGet(i));
  jsonDoc["error"] = false;
  String json;
  serializeJson(jsonDoc, json);
  webServer.send(200, "application/json", json);
}

static void apiSwitchPost()
{
  jsonDoc.clear();
  deserializeJson(jsonDoc, webServer.arg("plain"));
  switchSet(jsonDoc["index"], jsonDoc["state"]);
  webServer.send(200, "application/json", "{}");
}

static void bootstrapCss()
{
  File file = SPIFFS.open("/web/bootstrap.css.gz", "r");
  size_t sent = webServer.streamFile(file, "text/css");
}

static void bootstrapJs()
{
  File file = SPIFFS.open("/web/bootstrap.js.gz", "r");
  size_t sent = webServer.streamFile(file, "application/javascript");
}

static void feather()
{
  File file = SPIFFS.open("/web/feather.js.gz", "r");
  size_t sent = webServer.streamFile(file, "application/javascript");
}

static void jquery()
{
  File file = SPIFFS.open("/web/jquery.js.gz", "r");
  size_t sent = webServer.streamFile(file, "application/javascript");
}

static void popper()
{
  File file = SPIFFS.open("/web/popper.js.gz", "r");
  size_t sent = webServer.streamFile(file, "application/javascript");
}

static void scripts()
{
  File file = SPIFFS.open("/web/scripts.js.gz", "r");
  size_t sent = webServer.streamFile(file, "application/javascript");
}

static void style()
{
  File file = SPIFFS.open("/web/style.css.gz", "r");
  size_t sent = webServer.streamFile(file, "text/css");
}

static void styles()
{
  File file = SPIFFS.open("/web/styles.css.gz", "r");
  size_t sent = webServer.streamFile(file, "text/css");
}

void setupWebServer()
{
  webServer.on("/", HTTP_GET, root);
  webServer.on("/about", HTTP_GET, about);
  webServer.on("/settings", HTTP_GET, settings);
  webServer.on("/settings/device/", HTTP_GET, settingsDevice);
  webServer.on("/settings/wifi/", HTTP_GET, settingsWifi);
  webServer.on("/settings/ap/", HTTP_GET, settingsAp);
  webServer.on("/settings/mqtt/", HTTP_GET, settingsMqtt);

  webServer.on("/api/device", HTTP_GET, apiDeviceGet);
  webServer.on("/api/wifi", HTTP_GET, apiWifiGet);
  webServer.on("/api/ap", HTTP_GET, apiApGet);
  webServer.on("/api/mqtt", HTTP_GET, apiMqttGet);
  webServer.on("/api/switches", HTTP_GET, apiSwitchGet);

  webServer.on("/api/device", HTTP_POST, apiDevicePost);
  webServer.on("/api/wifi", HTTP_POST, apiWifiPost);
  webServer.on("/api/ap", HTTP_POST, apiApPost);
  webServer.on("/api/mqtt", HTTP_POST, apiMqttPost);
  webServer.on("/api/switches", HTTP_POST, apiSwitchPost);
  
  webServer.on("/assets/bootstrap.min.css", HTTP_GET, bootstrapCss);
  webServer.on("/assets/bootstrap.min.js", HTTP_GET, bootstrapJs);
  webServer.on("/assets/feather.min.js", HTTP_GET, feather);
  webServer.on("/assets/jquery-3.3.1.min.js", HTTP_GET, jquery);
  webServer.on("/assets/popper.min.js", HTTP_GET, popper);
  webServer.on("/assets/scripts.js", HTTP_GET, scripts);
  webServer.on("/assets/style.css", HTTP_GET, style);
  webServer.on("/assets/styles.css", HTTP_GET, styles);

  webServer.onNotFound(handleNotFound);
  webServer.begin();

  jsonBuffer[JSON_BUFFER_SIZE] = 0;
}

void webServerLoop()
{
  webServer.handleClient();
}
