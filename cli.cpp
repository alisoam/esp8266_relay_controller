#include "command_line.hpp"

#include <time.h>
#include <cstring>

#include "mqtt.hpp"
#include "parameters.hpp"
#include "peripherals.hpp"

static CommandLine cmd;

static std::string wifiCmd(const std::vector<std::string>& args)
{
  std::stringstream IO_;
  if (args.size() == 0 or args.at(0) == "help")
  {
    IO_ << "wifi get" << std::endl
        << "wifi active" << std::endl
        << "wifi deactive" << std::endl
        << "wifi set ssid [SSID]" << std::endl
        << "wifi set password [PASSWORD]";
  }
  else if (args.at(0) == "get")
  {
    IO_ << "wifi: " << getWifiSsid() << ":" << getWifiPassword() << '(' << (getWifiActive()? "active": "deactive") << (getWifiActive()? (WiFi.isConnected()? ", connected": ", not connected"): "") << ')';
  }
  else if (args.at(0) == "active")
  {
    setWifiActive(true);
    IO_ << "WIFI is active";
  }
  else if (args.at(0) == "deactive")
  {
    setWifiActive(false);
    IO_ << "WIFI is deactive";
  }
  else if (args.at(0) == "set")
  {
    std::string arg;
    for (unsigned int i = 2; i < args.size(); i++)
      arg += args.at(i) + " ";
    arg.pop_back();
    if (args.at(1) == "ssid")
    {
      setWifiSsid(arg.c_str());
      IO_ << "WIFI SSID set to: \"" << arg << "\".";
    }
    else if (args.at(1) == "password")
    {
      setWifiPassword(arg.c_str());
      IO_ << "WIFI password set to: \"" << arg.c_str() << "\".";
    }
  }
  return IO_.str();
}

static std::string apCmd(const std::vector<std::string>& args)
{
  std::stringstream IO_;
  if (args.size() == 0 or args.at(0) == "help")
    IO_ << "ap get" << std::endl
        << "ap active" << std::endl
        << "ap deactive" << std::endl
        << "ap set ssid [SSID]" << std::endl
        << "ap set password [PASSWORD]";
  else if (args.at(0) == "get")
    IO_ << "ap: " << getApSsid() << ":" << getApPassword() << (getApActive()? "(active)": "(deactive)");
  else if (args.at(0) == "active")
  {
    setApActive(true);
    IO_ << "AP is active";
  }
  else if (args.at(0) == "deactive")
  {
    setApActive(false);
    IO_ << "AP is deactive";
  }
  else if (args.at(0) == "set")
  {
    std::string arg;
    for (unsigned int i = 2; i < args.size(); i++)
      arg += args.at(i) + " ";
    arg.pop_back();
    if (args.at(1) == "ssid")
    {
      setApSsid(arg.c_str());
      IO_ << "AP SSID set to: \"" << arg << "\".";
    }
    else if (args.at(1) == "password")
    {
      if (arg.length() >= 8)
      {
        setApPassword(arg.c_str());
        IO_ << "AP password set to: \"" << arg.c_str() << "\".";
      }
      else
        IO_ << "AP password should be longer than 8 character.";
    }
  }
  return IO_.str();
}

static std::string mqttCmd(const std::vector<std::string>& args)
{
  std::stringstream IO_;
  if (args.size() == 0 or args.at(0) == "help")
  {
    IO_ << "mqtt get" << std::endl
        << "mqtt set username [USERNAME]" << std::endl
        << "mqtt set password [PASSWORD]" << std::endl
        << "mqtt set hostname [HOSTNAME]" << std::endl
        << "mqtt set port [PORT]";
  }
  else if (args.at(0) == "get")
  {
    IO_ << "mqtt: " << '(' << (getMqttActive()? "active": "deactive") << (getMqttActive()? (mqttClient.connected()? ", connected": ", not connected"): "") << ')' << std::endl
        << "\tHost: \"" << getMqttHostname() << ":" << getMqttPort() << "\"" << std::endl
        << "\tUsername: \"" << getMqttUsername() << "\"" <<std::endl
        << "\tPassword: \"" << getMqttPassword() << "\"" <<std::endl;
  }
  else if (args.at(0) == "active")
  {
    setMqttActive(true);
    IO_ << "MQTT is active";
  }
  else if (args.at(0) == "deactive")
  {
    setMqttActive(false);
    IO_ << "MQTT is deactive";
  }
  else if (args.at(0) == "set")
  {
    std::string arg;
    for (unsigned int i = 2; i < args.size(); i++)
      arg += args.at(i) + " ";
    arg.pop_back();
    if (args.at(1) == "hostname")
    {
      setMqttHostname(arg.c_str());
      IO_ << "MQTT hostname set to: \"" << arg << "\".";
    }
    else if (args.at(1) == "port")
    {
      setMqttPort(atoi(arg.c_str()));
      IO_ << "MQTT port set to: \"" << arg << "\".";
    }
    else if (args.at(1) == "username")
    {
      setMqttUsername(arg.c_str());
      IO_ << "MQTT username set to: \"" << arg << "\".";
    }
    else if (args.at(1) == "password")
    {
      setMqttPassword(arg.c_str());
      IO_ << "MQTT password set to: \"" << arg << "\".";
    }
  }
  return IO_.str();
}

static std::string deviceCmd(const std::vector<std::string>& args)
{
  std::stringstream IO_;
  if (args.size() == 0 or args.at(0) == "help")
  {
    IO_ << "device get" << std::endl
        << "device set name [DEVICENAME]" << std::endl
        << "device set time [YYYY-MM-DD hh:mm:ss]";
  }
  else if (args.at(0) == "get")
  {
    IO_ << "device: " << std::endl
        << "\tName: \"" << getDeviceName() << "\"" << std::endl;
    DateTime t = rtc.now();
    IO_ << "\tTime: " << (int)(t.year()) << "-" << (int)(t.month()) << "-" << (int)(t.day()) << " "
        << (int)(t.hour()) << ":" << (int)(t.minute()) << ":" << (int)(t.second());
  }
  else if (args.at(0) == "set")
  {
    std::string arg;
    for (unsigned int i = 2; i < args.size(); i++)
      arg += args.at(i) + " ";
    arg.pop_back();
    if (args.at(1) == "name")
    {
      setDeviceName(arg.c_str());
      IO_ << "Device name set to: \"" << arg << "\".";
    }
    if (args.at(1) == "time")
    {
      struct tm tm;
      strptime(arg.c_str(), "%Y-%m-%d %H:%M:%S", &tm);
      rtc.adjust(DateTime(tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec));
    }
  }
  return IO_.str();
}

void setupCli()
{
  cmd.addCommand("wifi", wifiCmd);
  cmd.addCommand("ap", apCmd);
  cmd.addCommand("mqtt", mqttCmd);
  cmd.addCommand("device", deviceCmd);
}

void cliLoop()
{
  static std::string line;
  while (Serial.available() > 0)
  {
    int incomingByte = Serial.read();
    if (incomingByte < 0 or incomingByte == '\r')
      continue;
    else if (incomingByte != '\n' && line.length() < 70)
      line += incomingByte;
    else
    {
      try
      {
        if (line.length() > 0)
          Serial.println(cmd.execute(line + " ").c_str());
        Serial.print(">> ");
        //Serial.flush();
      }
      catch (std::exception& e)
      {
        Serial.print("Error: ");
        Serial.println(e.what());
      }
      line = "";
    }
  }
}
