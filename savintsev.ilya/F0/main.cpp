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
    try
    {
      read_savi_file(argv[i], projects);
    }
    catch (const std::runtime_error & e)
    {
      std::cerr << e.what() << '\n';
    }
  }

  std::cout << "Welcome to the CMD-PAINT\n";

  std::map< std::string, std::function< void() > > cmds;

  cmds["open"] = std::bind(open, std::ref(std::cin), std::ref(std::cout), std::ref(projects));
  cmds["close"] = std::bind(close, std::ref(std::cin), std::ref(std::cout), std::ref(projects));
  cmds["create"] = std::bind(create, std::ref(std::cin), std::ref(std::cout), std::ref(projects));
  cmds["save"] = std::bind(save, std::ref(std::cin), std::ref(std::cout), std::ref(projects));
  cmds["save_as"] = std::bind(save_as, std::ref(std::cin), std::ref(std::cout), std::ref(projects));
  cmds["save_all"] = std::bind(save_all, std::ref(std::cout), std::ref(projects));
  cmds["copy"] = std::bind(copy, std::ref(std::cin), std::ref(std::cout), std::ref(projects));
  cmds["merge"] = std::bind(merge, std::ref(std::cin), std::ref(std::cout), std::ref(projects));
  cmds["rename"] = std::bind(rename_project, std::ref(std::cin), std::ref(std::cout), std::ref(projects));
  cmds["projects"] = std::bind(print, std::ref(std::cout), std::ref(projects));

  for (std::string command; std::cin >> command;)
  {
    try
    {
      cmds.at(command)();
    }
    catch (const std::runtime_error & e)
    {
      if (std::cin.fail())
      {
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cerr << e.what() << '\n';
    }
    catch (const std::out_of_range & e)
    {
      if (std::cin.fail())
      {
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cerr << e.what() << '\n';
    }
    catch (...)
    {
      if (std::cin.fail())
      {
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cerr << "Unknown error\n";
    }
  }
}
