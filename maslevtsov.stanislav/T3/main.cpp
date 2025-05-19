#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include "shapes.hpp"
#include "commands.hpp"

int main()
{
  // int argc, char** argv
  using namespace maslevtsov;

  // if (argc != 2) {
  //   std::cerr << "<INVALID PARAMETERS NUMBER>\n";
  //   return 1;
  // }
  // std::ifstream fin(argv[1]);
  // if (!fin) {
  //   std::cerr << "<INVALID DATA FILE>\n";
  //   return 1;
  // }
  std::vector< Polygon > polygons;
  // while (!fin.eof()) {
  //   std::copy(std::istream_iterator< Polygon >(fin), std::istream_iterator< Polygon >(),
  //   std::back_inserter(polygons)); if (fin.fail()) {
  //     fin.clear();
  //     fin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  //   }
  // }

  Polygon polygon1;
  Polygon polygon2;
  Polygon polygon3;
  Polygon polygon4;
  Polygon polygon5;
  Polygon polygon6;
  Polygon polygon7;
  polygon1.points = {{1, 1}, {2, 2}, {1, 2}};
  polygon2.points = {{0, 0}, {0, 4}, {2, 6}, {4, 6}, {6, 4}, {6, 2}, {4, 0}};
  polygon3.points = {{0, 0}, {1, 0}, {0, 1}};
  polygon4.points = {{0, 0}, {1, 0}, {0, 1}};
  polygon5.points = {{0, 0}, {0, 4}, {2, 6}, {4, 6}, {6, 4}, {6, 2}, {4, 0}};
  polygon6.points = {{0, 0}, {0, 1}, {1, 0}};
  polygon7.points = {{1, 1}, {2, 2}, {1, 2}};
  polygons.push_back(polygon1);
  polygons.push_back(polygon2);
  polygons.push_back(polygon3);
  polygons.push_back(polygon4);
  polygons.push_back(polygon5);
  polygons.push_back(polygon6);
  polygons.push_back(polygon7);

  std::map< std::string, std::function< void(std::istream&, std::ostream&) > > commands;
  commands["AREA"] = std::bind(get_area, std::cref(polygons), std::placeholders::_1, std::placeholders::_2);
  commands["MAX"] = std::bind(get_max, std::cref(polygons), std::placeholders::_1, std::placeholders::_2);
  commands["MIN"] = std::bind(get_min, std::cref(polygons), std::placeholders::_1, std::placeholders::_2);
  commands["COUNT"] = std::bind(count_vertexes, std::cref(polygons), std::placeholders::_1, std::placeholders::_2);
  commands["ECHO"] = std::bind(echo, std::ref(polygons), std::placeholders::_1, std::placeholders::_2);
  commands["RMECHO"] = std::bind(remove_echo, std::ref(polygons), std::placeholders::_1, std::placeholders::_2);
  std::string command;
  while (std::cin >> command) {
    try {
      commands.at(command)(std::cin, std::cout);
      std::cout << '\n';
    } catch (...) {
      std::cout << "<INVALID COMMAND>\n";
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
}
