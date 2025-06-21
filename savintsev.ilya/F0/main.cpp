#include <iostream>
#include <fstream>
#include <functional>
#include <limits>
#include <string>
#include <map>
#include <list>
#include <shape-utils.hpp>
#include <shape.hpp>

int main(int argc, char * argv[])
{
  using namespace savintsev;

  std::map< std::string, std::list< std::pair< std::string, Shape * > > > projects;

  for (size_t i = 1; i < argc; ++i)
  {
    std::ifstream file(argv[i]);
    if (!file)
    {
      std::cerr << "ERROR: Can't open file\n";
      continue;
    }

    while (!file.eof())
    {
      std::list< std::pair< std::string, Shape * > > project;
      std::string temp;
      
      createShape(temp, )
      if (!file.fail())
      {
        data.push_back(p);
        continue;
      }
      file.clear(file.rdstate() ^ std::ios::failbit);
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::map< std::string, std::function< void() > > cmds;
  cmds["open"] = std::bind(area, std::ref(std::cin), std::ref(std::cout), std::cref(data));
  cmds["close"] = std::bind(max, std::ref(std::cin), std::ref(std::cout), std::cref(data));
  cmds["create"] = std::bind(min, std::ref(std::cin), std::ref(std::cout), std::cref(data));
  cmds["copy"] = std::bind(count, std::ref(std::cin), std::ref(std::cout), std::cref(data));
  cmds["rename"] = std::bind(intersections, std::ref(std::cin), std::ref(std::cout), std::cref(data));
  cmds["save"] = std::bind(same, std::ref(std::cin), std::ref(std::cout), std::cref(data));
  cmds["save_as"] = std::bind(print, std::ref(std::cout), std::cref(data));
  cmds["save_all"] = std::bind(print, std::ref(std::cout), std::cref(data));
  cmds["render"] = std::bind(print, std::ref(std::cout), std::cref(data));
  cmds["merge"] = std::bind(print, std::ref(std::cout), std::cref(data));
  cmds["relocate"] = std::bind(print, std::ref(std::cout), std::cref(data));
  cmds["projects"] = std::bind(print, std::ref(std::cout), std::cref(data));

  for (std::string command; std::cin >> command;)
  {
    try
    {
      cmds.at(command)();
    }
    catch(...)
    {
      if (std::cin.fail())
      {
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}
