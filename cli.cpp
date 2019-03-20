#include "command_line.hpp"

#include <cstring>

#include "parameters.hpp"
#include "peripherals.hpp"

static CommandLine cmd;

static std::string timeCmd(const std::vector<std::string>& args)
{
  std::stringstream IO_;
  if (args.size() == 0 or args.at(0) == "help")
  {
    IO_ << "time get" << std::endl
        << "time set yyyy mm dd HH MM SS";
  }
  else if (args.at(0) == "get")
  {
    DateTime t = rtc.now();
    IO_ << "Now: " << (int)(t.year()) << "-" << (int)(t.month()) << "-" << (int)(t.day()) << " "
        << (int)(t.hour()) << ":" << (int)(t.minute()) << ":" << (int)(t.second());
  }
  else if (args.at(0) == "set")
  {
     rtc.adjust(DateTime(atoi(args.at(1).c_str()), atoi(args.at(2).c_str()), atoi(args.at(3).c_str()), atoi(args.at(4).c_str()), atoi(args.at(5).c_str()), atoi(args.at(6).c_str())));
  }
  return IO_.str();
}

static std::string wifiCmd(const std::vector<std::string>& args)
{
  std::stringstream IO_;
  if (args.size() == 0 or args.at(0) == "help")
  {
    IO_ << "wifi get" << std::endl
        << "wifi set ssid [SSID] << std::endl"
        << "wifi set password [PASSWORD]";
  }
  else if (args.at(0) == "get")
  {
    IO_ << "wifi: " << getWifiSsid() << ":" << getWifiPassword();
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
      IO_ << "WIFI SSID set to: \"" << arg << "\". You should restart the system.";
    }
    else if (args.at(1) == "password")
    {
      setWifiPassword(arg.c_str());
      IO_ << "WIFI password set to: \"" << arg.c_str() << "\". You should restart the system.";
    }
  }
  return IO_.str();
}

static std::string apCmd(const std::vector<std::string>& args)
{
  std::stringstream IO_;
  if (args.size() == 0 or args.at(0) == "help")
  {
    IO_ << "ap get" << std::endl
        << "ap set ssid [SSID] << std::endl"
        << "ap set password [PASSWORD]";
  }
  else if (args.at(0) == "get")
  {
    IO_ << "ap: " << getApSsid() << ":" << getApPassword();
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
      setApPassword(arg.c_str());
      IO_ << "AP password set to: \"" << arg.c_str() << "\".";
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
    IO_ << "mqtt: " << std::endl
        << "\tHost: \"" << getMqttHostname() << ":" << getMqttPort() << "\"" << std::endl
        << "\tUsername: \"" << getMqttUsername() << "\"" <<std::endl
        << "\tPassword: \"" << getMqttPassword() << "\"" <<std::endl;
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
        << "device set name [DEVICENAME]";
  }
  else if (args.at(0) == "get")
  {
    IO_ << "device: " << std::endl
        << "\tName: \"" << getDeviceName() << "\"";
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
  }
  return IO_.str();
}

void setupCli()
{
  cmd.addCommand("time", timeCmd);
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
