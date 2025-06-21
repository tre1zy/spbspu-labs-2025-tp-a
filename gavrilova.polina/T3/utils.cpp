#include "utils.hpp"
#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <locale>
#include <sstream>
#include "commands.hpp"

std::vector< std::string > gavrilova::tokenize(std::vector< std::string >& tokens, const std::string& str)
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

void gavrilova::readFile(const std::string& filename, std::vector< Polygon >& polygons)
{
  if (filename.empty()) {
    throw std::runtime_error("Filename is empty");
  }

  std::ifstream input_file(filename);
  if (!input_file) {
    throw std::runtime_error("Could not open file " + filename);
  }

  std::copy(std::istream_iterator< Polygon >(input_file),
      std::istream_iterator< Polygon >(),
      std::back_inserter(polygons));
}

void gavrilova::loadPolygons(const std::string& filename, std::vector< Polygon >& polygons, std::ostream& out)
{
  readFile(filename, polygons);
  std::copy(
      polygons.begin(),
      polygons.end(),
      std::ostream_iterator< Polygon >(out, "\n"));
}

void gavrilova::startCommandInterface(const std::string& filename, std::istream& is, std::ostream& out)
{
  std::vector< Polygon > polygons;
  try {
    loadPolygons(filename, polygons, out);
  } catch (const std::exception& e) {
    throw;
  }

  std::vector< std::string > command_tokens;
  static std::map< std::string, std::function< void(std::ostream&) > > command_map = {
      {"AREA", [&polygons, &command_tokens](std::ostream& out) {
         processArea(polygons, command_tokens, out);
       }},
      {"MIN", [&polygons, &command_tokens](std::ostream& out) {
         processMinMax(polygons, command_tokens, out);
       }},
      {"MAX", [&polygons, &command_tokens](std::ostream& out) {
         processMinMax(polygons, command_tokens, out);
       }},
      {"COUNT", [&polygons, &command_tokens](std::ostream& out) {
         processCount(polygons, command_tokens, out);
       }},
      {"PERMS", [&polygons, &command_tokens](std::ostream& out) {
         processPerms(polygons, command_tokens, out);
       }},
      {"LESSAREA", [&polygons, &command_tokens](std::ostream& out) {
         processLessArea(polygons, command_tokens, out);
       }}};

  std::string input_command;
  while (std::getline(is, input_command)) {
    try {
      auto tokens = tokenize(command_tokens, input_command);
      if (tokens.empty()) {
        continue;
      }
      auto it = command_map.find(tokens[0]);
      if (it == command_map.end()) {
        out << "<INVALID COMMAND>";
        // throw std::runtime_error("Invalid command");
      }
      it->second(out);
    } catch (const std::exception&) {
      throw;
    }
  }
}
