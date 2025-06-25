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

  cmds["rectangle"] = std::bind(create_rectangle, std::ref(std::cin), std::ref(std::cout), std::ref(projects));
  cmds["complexquad"] = std::bind(create_complexquad, std::ref(std::cin), std::ref(std::cout), std::ref(projects));
  cmds["concave"] = std::bind(create_concave, std::ref(std::cin), std::ref(std::cout), std::ref(projects));
  cmds["delete"] = std::bind(delete_shape, std::ref(std::cin), std::ref(std::cout), std::ref(projects));

  cmds["back_up"] = std::bind(make_back);
  for (std::string command; std::cin >> command;)
  {
    try
    {
      cmds.at(command)();
    }
    catch (const std::bad_alloc &)
    {

    struct Deleter
    {
      void operator()(Layer & layer) const
      {
        delete layer.second;
      }
    };

    struct ProjectProcessor
    {
      void operator()(std::pair< const std::string, Project > & proj_pair) const
      {
        Project & proj = proj_pair.second;
        write_savi_file(proj_pair.first + "_backup", proj);
        std::for_each(proj.begin(), proj.end(), Deleter());
        proj.clear();
      }
    };

    std::for_each(projects.begin(), projects.end(), ProjectProcessor());

    std::cerr << "Memory allocation failed. Open data were saved to backup files. Exiting.\n";
    return 1;
    }
    catch (const std::out_of_range &)
    {
      std::cout << "There is no such command or project\n";
    }
    catch (const std::runtime_error & e)
    {
      std::cout << e.what() << '\n';
    }
    catch (...)
    {
      std::cerr << "Unknown error. Exiting\n";
      return 1;
    }

    if (std::cin.fail())
    {
      std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }

  return 0;
}
