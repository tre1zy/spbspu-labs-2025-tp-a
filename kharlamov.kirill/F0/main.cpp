#include <fstream>
#include <iostream>
#include <string>
#include "commands.h"

int main(int argc, char* argv[])
{
  kharlamov::CommandProcessor processor;

  if (argc > 1)
  {
    std::string option = argv[1];
    if (option == "--help")
    {
      std::cout << processor.help() << std::endl;
      return 0;
    }
    else if (argc == 3)
    {
      std::ifstream inputFile(argv[2]);
      if (!inputFile.is_open())
      {
        std::cerr << "Error opening input file" << std::endl;
        return 1;
      }

      std::string line;
      while (std::getline(inputFile, line))
      {
        std::string result = processor.processCommand(line);
        std::cout << result << std::endl;
      }

      return 0;
    }
  }

  std::cout << "English-Russian Dictionary (2-3 tree implementation)" << std::endl;
  std::cout << "Type 'help' for list of commands or 'exit' to quit" << std::endl;

  std::string command;
  while (true)
  {
    std::cout << "> ";
    std::getline(std::cin, command);

    if (command == "exit") break;

    std::string result = processor.processCommand(command);
    std::cout << result << std::endl;
  }

  return 0;
}
