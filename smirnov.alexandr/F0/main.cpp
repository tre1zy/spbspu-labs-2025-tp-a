#include <iostream>
#include <functional>
#include <limits>
#include <guard.hpp>
#include <delimetr.hpp>
#include "dictionary.hpp"

int main()
{
  using namespace smirnov;
  Dicts dicts;
  std::unordered_map< std::string, std::function< void(std::istream &) > > commands;
  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      std::cout << "Команда: " << cmd << "\n";
    }
    catch (const std::exception &)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
}
