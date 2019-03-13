#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

#include <map>
#include <list>
#include <string>
#include <vector>
#include <cstring>
#include <sstream>
#include <iostream>
#include <exception>
#include <functional>

typedef std::vector<std::string> ArgumentsList;

using CommandFunction = std::function<std::string(const ArgumentsList& )> ;

class CommandLineException: public std::exception
{
public:
  CommandLineException(const char* what)
  {
    std::strncpy(what_, what, 40);
    what_[39] = 0;
  }
  virtual const char* what() const noexcept
  {
	return what_;
  }
private:
  char what_[40];
};

class CommandLine
{
public:
  void addCommand(std::string commandName, CommandFunction function)
  {
    functions.emplace(commandName, function);
  }
  std::string execute(std::string line)
  {
    std::string command;
    std::string argument;
    std::vector<std::string> arguments;
    std::stringstream lineStream(line + " ");
    if (!(lineStream >> command))
      throw CommandLineException("Bad command");
    while (lineStream >> argument)
      arguments.emplace_back(argument);
    try
    {
      if (command == "help")
      {
        std::string helpString = "Available commands:\n";
        for (auto& command: functions)
          helpString += command.first;
        return "";
      }
      return functions.at(command)(arguments);
    }
    catch (CommandLineException&)
    {
      throw;
    }
    catch (...)
    {
      throw CommandLineException("Command not found");
    }
  }
private:
  std::map<std::string, CommandFunction> functions;
};

#endif
