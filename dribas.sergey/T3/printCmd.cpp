#include "printCmd.hpp"
#include <map>
#include <functional>
#include <algorithm>

namespace
{
  bool isEven(const dribas::Poligon& plg)
  {
    return plg.points.size() % 2;
  }
  bool isnEven(const dribas::Poligon& plg)
  {
    return !isEven(plg);
  }
}

namespace dribas
{
  std::ostream printArea(const std::vector< Poligon >& plg, std::istream& in)
  {
    if (!plg.size()) {
      throw std::out_of_range("No figure found");
    }
    std::string command;
    std::map< std::string, std::function< void() > > cmds;
    cmds["show"] = std::bind(std::copy_if, plg.cbegin(), plg.cend(), std::back_inserter(plg), isEven);

    in >> command;
    if (command == "<EVEN>") {

    } else if (command == "<ODD>") {

    } else if (command == "<MEAN>") {

    }
  }
}