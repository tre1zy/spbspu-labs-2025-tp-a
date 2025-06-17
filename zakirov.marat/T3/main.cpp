#include <list>
#include <map>
#include <functional>
#include <fstream>
#include "geometric_shape.hpp"
#include "area_processing.hpp"
#include "vertex_processing.hpp"
#include "extremum_processing.hpp"
#include "utilities.hpp"

int main(int argc, char ** argv)
{
  if (argc != 2)
  {
    std::cerr << "<INVALID PARAMETER>" << '\n';
    return 1;
  }

  std::ifstream fin(argv[1]);

  std::list< zakirov::Polygon > shapes;
  std::map< std::string, std::function< void(std::istream &, std::ostream &) > > commands;
  commands["AREA"] = std::bind(zakirov::process_area, std::cref(shapes), std::placeholders::_1, std::placeholders::_2);
  commands["LESSAREA"] = std::bind(zakirov::process_less_area, std::cref(shapes), std::placeholders::_1, std::placeholders::_2);
  commands["MAX"] = std::bind(zakirov::find_max_extremum, std::cref(shapes), std::placeholders::_1, std::placeholders::_2);
  commands["MIN"] = std::bind(zakirov::find_min_extremum, std::cref(shapes), std::placeholders::_1, std::placeholders::_2);
  commands["COUNT"] = std::bind(zakirov::count, std::cref(shapes), std::placeholders::_1, std::placeholders::_2);
  commands["ECHO"] = std::bind(zakirov::echo, std::ref(shapes), std::placeholders::_1, std::placeholders::_2);

  std::string command;
  while (!(std::cin >> command).eof())
    {
    try
    {
      commands.at(command)(std::cin, std::cout);
      std::cout << '\n';
    }
    catch (const std::exception & e)
    {
      std::cout << e.what() << '\n';
    }
  }
}
