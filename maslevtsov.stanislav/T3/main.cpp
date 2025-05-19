#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
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
  while (!fin.eof()) {
    if (fin.fail()) {
      fin.clear();
      fin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    std::copy(std::istream_iterator< Polygon >(fin), std::istream_iterator< Polygon >(), std::back_inserter(polygons));
  }
  std::map< std::string, std::function< void(std::istream&, std::ostream&) > > commands;
  using namespace std::placeholders;
  commands["AREA"] = std::bind(get_area, std::cref(polygons), _1, _2);
  commands["MAX"] = std::bind(get_max, std::cref(polygons), _1, _2);
  commands["MIN"] = std::bind(get_min, std::cref(polygons), _1, _2);
  commands["COUNT"] = std::bind(count_vertexes, std::cref(polygons), _1, _2);
  commands["ECHO"] = std::bind(echo, std::ref(polygons), _1, _2);
  commands["RMECHO"] = std::bind(remove_echo, std::ref(polygons), _1, _2);
  std::string command;
  while (!(std::cin >> command).eof()) {
    try {
      commands.at(command)(std::cin, std::cout);
      std::cout << '\n';
    } catch (...) {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
