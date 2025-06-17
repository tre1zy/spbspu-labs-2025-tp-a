#include <list>
#include <map>
#include <functional>
#include <fstream>
#include "geometric_shape.hpp"
#include "line_commands.hpp"

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
  commands["MAX"] = std::bind(zakirov::find_extremum_max, std::cref(shapes), std::placeholders::_1, std::placeholders::_2);
  commands["MIN"] = std::bind(zakirov::find_extremum_min, std::cref(shapes), std::placeholders::_1, std::placeholders::_2);
}
