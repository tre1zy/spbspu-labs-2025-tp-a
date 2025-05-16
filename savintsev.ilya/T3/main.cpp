#include <iostream>
#include <functional>
#include <limits>
#include <string>
#include <map>

int main()
{
  std::map< std::string, std::function< void() > > cmds;

  for (std::string command; std::cin >> command;)
  {
    try
    {
      cmds.at(command)();
    }
    catch(...)
    {
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}
