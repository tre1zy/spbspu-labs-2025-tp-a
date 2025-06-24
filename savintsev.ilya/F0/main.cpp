#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <functional>
#include <limits>
#include <shape-utils.hpp>
#include "project-body.hpp"
#include "project-cmds.hpp"
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
      std::cerr << "ERROR: Can't open " << get_filename_wext(argv[i]) << '\n';
      continue;
    }

    Project project;

    while (!file.eof())
    {
      std::string figure;
      file >> figure;
      Layer new_pair;
      Shape * new_shape = createShape(file, figure);
      if (!new_shape)
      {
        std::cerr << "ERROR: Unknown error. Invalid file " << get_filename_wext(argv[i]) << '\n';
        return 1;
      }
      new_pair = {figure, new_shape};
      project.push_back(new_pair);
    }

    projects[get_filename(argv[i])] = project;
  }

  std::cout << "Welcome to the CMD-PAINT\n";

  std::map< std::string, std::function< void() > > cmds;

  cmds["open"] = std::bind(open, std::ref(std::cin), std::ref(std::cout), std::ref(projects));
  cmds["close"] = std::bind(close, std::ref(std::cin), std::ref(std::cout), std::ref(projects));
  cmds["create"] = std::bind(create, std::ref(std::cin), std::ref(std::cout), std::ref(projects));
  cmds["save"] = std::bind(save, std::ref(std::cin), std::ref(std::cout), std::ref(projects));
  cmds["save_as"] = std::bind(save_as, std::ref(std::cin), std::ref(std::cout), std::ref(projects));
  cmds["projects"] = std::bind(print, std::ref(std::cout), std::ref(projects));

  for (std::string command; std::cin >> command;)
  {
    try
    {
      cmds.at(command)();
    }
    catch (const std::out_of_range & e)
    {
      if (std::cin.fail())
      {
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cerr << e.what();
    }
    catch (...)
    {
      if (std::cin.fail())
      {
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
