#include "Utils.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <locale>
#include <sstream>
#include "Commands/Commands.hpp"

namespace gavrilova {

  std::vector< std::string > tokenize(std::vector< std::string >& tokens, const std::string& str)
  {
    std::istringstream iss(str);

    if (!tokens.empty()) {
      tokens.clear();
    }

    std::string token = {};
    while (iss >> token) {
      std::transform(
          token.begin(),
          token.end(),
          token.begin(),
          std::bind(
              std::toupper< char >,
              std::placeholders::_1,
              std::locale()));
      tokens.push_back(token);
    }
    return tokens;
  }

  void readFile(const std::string& filename, std::vector< Polygon >& polygons)
  {
    if (filename.empty()) {
      return;
    }

    std::ifstream input_file(filename);
    if (!input_file) {
      throw std::runtime_error("Error: Could not open file " + filename);
    }

    std::copy(std::istream_iterator< Polygon >(input_file),
        std::istream_iterator< Polygon >(),
        std::back_inserter(polygons));
  }

  void loadPolygons(const std::string& filename, std::vector< Polygon >& polygons)
  {
    try {
      readFile(filename, polygons);
    } catch (...) {
      std::cout << "Reading file failed" << std::endl;
      throw;
    }

    std::copy(
        polygons.begin(),
        polygons.end(),
        std::ostream_iterator< Polygon >(std::cout, "\n"));

    std::cout << "Initialization succeed. " << std::endl;
  }

  void startCommandInterface(const std::string& filename, std::istream& is)
  {
    std::vector< Polygon > polygons = {};
    loadPolygons(filename, polygons);

    std::vector< std::string > command_tokens = {};
    static std::map< std::string, std::function< void() > > command_map = {
        {"AREA", std::bind(processArea, std::cref(polygons), std::ref(command_tokens))},
        {"MIN", std::bind(processMinMax, std::cref(polygons), std::ref(command_tokens))},
        {"MAX", std::bind(processMinMax, std::cref(polygons), std::ref(command_tokens))},
        {"COUNT", std::bind(processCount, std::cref(polygons), std::ref(command_tokens))},
        {"PERMS", std::bind(processPerms, std::cref(polygons), std::ref(command_tokens))},
        {"LESSAREA", std::bind(processLessArea, std::cref(polygons), std::ref(command_tokens))}};

    std::string input_command = {};
    while (std::getline(is, input_command) && input_command != STOP_KEYWORD) {
      auto tokens = tokenize(command_tokens, input_command);
      try {
        command_map.at(tokens[0])();
      } catch (...) {
        std::cout << "Invalid command." << std::endl;
      }
    }
  }
}
