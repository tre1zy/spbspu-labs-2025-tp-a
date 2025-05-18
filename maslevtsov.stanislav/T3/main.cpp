#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include "shapes.hpp"
#include "commands.hpp"

int main(int argc, char** argv)
{
  using namespace maslevtsov;

  if (argc != 2) {
    std::cerr << "<INVALID PARAMETERS NUMBER>\n";
    return 1;
  }
  std::ifstream fin(argv[1]);
  if (!fin) {
    std::cerr << "<INVALID DATA FILE>\n";
    return 1;
  }
  std::vector< Polygon > polygons;
  std::map< std::string, std::function< void(std::istream&, std::ostream&) > > commands;
  commands["AREA"] = std::bind(get_area, std::cref(polygons), std::placeholders::_1, std::placeholders::_2);
  commands["MAX"] = std::bind(get_max, std::cref(polygons), std::placeholders::_1, std::placeholders::_2);
  commands["MIN"] = std::bind(get_min, std::cref(polygons), std::placeholders::_1, std::placeholders::_2);
  commands["COUNT"] = std::bind(count_vertexes, std::cref(polygons), std::placeholders::_1, std::placeholders::_2);
  commands["ECHO"] = std::bind(echo, std::ref(polygons), std::placeholders::_1, std::placeholders::_2);
  commands["RMECHO"] = std::bind(remove_echo, std::ref(polygons), std::placeholders::_1, std::placeholders::_2);
  std::string command;
  while (!(fin >> command).eof()) {
    try {
      commands.at(command)(fin, std::cout);
    } catch (...) {
      if (fin.fail()) {
        fin.clear(fin.rdstate() ^ std::ios::failbit);
      }
      fin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}
