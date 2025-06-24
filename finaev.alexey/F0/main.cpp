#include <iostream>
#include <fstream>
#include <unordered_map>
#include <limits>
#include "commands.hpp"
#include "openningInfo.hpp"

int main(/*int argc, char* argv[]*/)
{
  /*if (argc != 2)
  {
    std::cerr << "Uncorrect input\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "Error openning\n";
    return 1;
  }*/
  std::unordered_map< std::string, finaev::OpenningInfo > globalOpenings;
  std::unordered_map< std::string, std::unordered_map< std::string, bool > > bases;
  auto commands = finaev::createCommandsHandler(std::cin, std::cout, globalOpenings/*, bases*/);
  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      commands.at(cmd)();
    }
    catch(std::exception& e)
    {
      if (std::cin.fail())
      {
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>";
    }
    std::cout << "\n";
  }
}
