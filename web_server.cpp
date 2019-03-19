#include "web_server.hpp"

#include <FS.h>
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>

#include "switches.hpp"

static ESP8266WebServer webServer(80);
StaticJsonDocument<500> jsonDoc;

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


static void apiWifiGet()
{
  char json[] = R"_({"active":true,"ssid":"abcdefghijklmno"})_";
  webServer.send(200, "application/json", json);
}

static void apiWifiPost()
{
  String message = "Body received:\n";
             message += webServer.arg("plain");
             message += "\n";;
  Serial.println(message);
  webServer.send(200, "application/json", "{}");
}

static void apiApGet()
{
  char json[] = R"_({"active":true,"ssid":"abcdefghijklmno"})_";
  webServer.send(200, "application/json", json);
}

static void apiApPost()
{
  Serial.println(webServer.arg("plain"));
  webServer.send(200, "application/json", "{}");
}

static void apiMqttGet()
{
  char json[] = R"_({"active":true, "secure":false,"hostname":"alisorouramini@gmail.com", "port":65123,"username":"abcdefghijklmno"})_";
  webServer.send(200, "application/json", json);
}

static void apiMqttPost()
{
  Serial.println(webServer.arg("plain"));
  webServer.send(200, "application/json", "{}");
}

static void apiSwitchGet()
{
  jsonDoc.clear();
  auto states = jsonDoc.createNestedArray("states");
  for (unsigned int i = 0; i < SWITCHES_NUMBER; i++)
    states.add(switchGet(i));
  jsonDoc["error"] = true;
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
  webServer.on("/settings/wifi/", HTTP_GET, settingsWifi);
  webServer.on("/settings/ap/", HTTP_GET, settingsAp);
  webServer.on("/settings/mqtt/", HTTP_GET, settingsMqtt);

  webServer.on("/api/wifi", HTTP_GET, apiWifiGet);
  webServer.on("/api/ap", HTTP_GET, apiApGet);
  webServer.on("/api/mqtt", HTTP_GET, apiMqttGet);
  webServer.on("/api/switches", HTTP_GET, apiSwitchGet);

  webServer.on("/api/wifi", HTTP_POST, apiWifiPost);
  webServer.on("/api/ap", HTTP_POST, apiApPost);
  webServer.on("/api/wifi", HTTP_POST, apiMqttPost);
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
}

void webServerLoop()
{
  webServer.handleClient();
}
