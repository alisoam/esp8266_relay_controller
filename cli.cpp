#include "command_line.hpp"

CommandLine cmd;

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
        << "wifi set SSID PASSWORD";
  }
  else if (args.at(0) == "get")
  {
  }
  else if (args.at(0) == "set")
  {
    
  }
  return IO_.str();
}

void cliSetup()
{
  cmd.addCommand("time", timeCmd);
  cmd.addCommand("wifi", wifiCmd);
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
