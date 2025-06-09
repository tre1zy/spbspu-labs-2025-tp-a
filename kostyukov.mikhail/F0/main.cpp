#include <exception>
#include <functional>
#include <ios>
#include <iostream>
#include <limits>
#include <map>
#include <stdexcept>
#include <string>

#include "freqDict.hpp"

int main()
{
  kostyukov::FreqDictManager dict_manager;
  std::map< std::string, std::function< void() > > commands;
  {
    using namespace std::placeholders;
    using namespace kostyukov;
    commands["create"] = std::bind(createDict, std::ref(std::cin), std::ref(std::cout), std::ref(dict_manager));
    commands["delete"] = std::bind(deleteDict, std::ref(std::cin), std::ref(std::cout), std::ref(dict_manager));
  }
  std::string command;
  while(!(std::cin >> command).eof())
  {
    try
    {
      commands.at(command)();
      std::cout << '\n';
    }
    catch(const std::out_of_range&)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
    catch(const std::exception& error)
    {
      std::cout << "<ERROR: " << error.what() << ">\n";
    }
    if (std::cin.fail())
    {
      std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;
}
