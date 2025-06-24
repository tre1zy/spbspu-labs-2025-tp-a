#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <functional>
#include <limits>
#include <shape-utils.hpp>
#include "project-body.hpp"
#include "file-system.hpp"

int main(int argc, char * argv[])
{
  using namespace savintsev;

  std::cout << "Starting...\n";

  Projects projects;

  for (int i = 1; i < argc; ++i)
  {
    std::ifstream file(argv[i]);
    if (!validate_savi_file(argv[i]))
    {
      std::cerr << "ERROR: Can't open file " << get_filename_wext(argv[i]) << '\n';
      continue;
    }

    Project project;

    while (!file.eof())
    {
      std::string figure;
      std::string name;
      file >> figure >> name;
      Layer new_pair;
      Shape * new_shape = createShape(file, figure);
      if (!new_shape)
      {
        std::cerr << "ERROR: Unknown error. Invalid file " << get_filename_wext(argv[i]) << '\n';
        return 1;
      }
      new_pair = {name, new_shape};
      project.push_back(new_pair);
    }

    projects[get_filename(argv[i])] = project;
  }

  for (auto it = projects.begin(); it != projects.end(); ++it)
  {
    std::cout << "Project " << it->first << '\n';
    for (auto jt = it->second.begin(); jt != it->second.end(); ++jt)
    {
      std::cout << jt->first << '\n';
    }
  }

  std::cout << "Welcome to the CMD-PAINT\n";

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
