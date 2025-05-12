#include <iostream>
#include <limits>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <map>
#include <functional>
#include "printCmd.hpp"
#include "poligon.hpp"

int main(int argc, char* argv[])
{
  using namespace dribas;

  if (argc != 2) {
    std::cerr << "no parametrs found" << '\n';
    return 1;
  }

  std::ifstream file(argv[1]);
  std::vector< dribas::Poligon > plgs;
  std::copy(std::istream_iterator<dribas::Poligon>(file),
          std::istream_iterator<dribas::Poligon>(),
          std::back_inserter(plgs));

  if (!file.is_open()) {
    std::cerr << "Error opening file: " << argv[1] << std::endl;
    return 1;
  }

  std::map< std::string, std::function< void() > > cmds;
  cmds["AREA"] = std::bind(printArea, std::cref(plgs), std::ref(std::cin), std::ref(std::cout));
  cmds["MAX"] = std::bind(printMax, std::cref(plgs), std::ref(std::cin), std::ref(std::cout));
  cmds["MIN"] = std::bind(printMin, std::cref(plgs), std::ref(std::cin), std::ref(std::cout));
  cmds["COUNT"] = std::bind(printCount, std::cref(plgs), std::ref(std::cin), std::ref(std::cout));
  cmds["LESSAREA"] = std::bind(printLessArea, std::cref(plgs), std::ref(std::cin), std::ref(std::cout));
  cmds["RIGHTSHAPES"] = std::bind(printRightShapes, std::cref(plgs), std::ref(std::cout));


  std::string command;
  while (!(std::cin >> command).eof())
  {
    try {
      cmds.at(command)();
      std::cout << "\n";
    } catch (std::exception& e) {
      if (std::cin.fail()) {
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cerr << "<INVALID COMMAND>\n";
    }
  }
  return 0;
}
