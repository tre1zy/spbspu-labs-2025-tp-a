#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <functional>
#include <limits>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <shape-utils.hpp>
#include <shape.hpp>
#include "validator.hpp"

int main(int argc, char * argv[])
{
  using namespace savintsev;

  std::map< std::string, std::list< std::pair< std::string, Shape * > > > projects;

  for (int i = 1; i < argc; ++i)
  {
    std::ifstream file(argv[i]);
    if (!validate_savi_file(argv[i]))
    {
      std::cerr << "ERROR: Can't open file " << argv[i] << '\n';
      continue;
    }

    std::list< std::pair< std::string, Shape * > > project;

    while (!file.eof())
    {
      std::string figure;
      std::string name;
      file >> figure >> name;
      std::pair< std::string, Shape * > new_pair;
      Shape * new_shape = createShape(file, figure);
      if (!new_shape)
      {
        std::cerr << "ERROR: Unknown error. Invalid file " << argv[i] << '\n';
        return 1;
      }
      new_pair = {name, new_shape};
      project.push_back(new_pair);
    }

    projects[argv[i]] = project;
  }

  for (auto it = projects.begin(); it != projects.end(); ++it)
  {
    std::cout << "Project " << it->first << '\n';
    for (auto jt = it->second.begin(); jt != it->second.end(); ++jt)
    {
      std::cout << jt->first << '\n';
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
