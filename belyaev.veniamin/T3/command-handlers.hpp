#ifndef COMMAND_HANDLERS_HPP
#define COMMAND_HANDLERS_HPP
#include <iomanip>
#include <iostream>
#include <map>
#include "command-helpers.hpp"
#include "shapes.hpp"
#include <stream-guard.hpp>

namespace belyaev
{
  double areaOdd(const std::vector<Polygon>& data);
  double areaEven(const std::vector<Polygon>& data);
  double areaMean(const std::vector<Polygon>& data);
  double areaVertices(const std::vector<Polygon>& data, size_t vertices);
  void area(const std::vector<Polygon>& data, std::istream& in, std::ostream& out);
  void minMaxArea(const std::vector<Polygon>& data, std::ostream& out, const std::string& command);
  void minMaxVertices(const std::vector<Polygon>& data, std::ostream& out, const std::string& command);
  void minMax(const std::vector<Polygon>& data, std::istream& in, std::ostream& out, const std::string& command);
  size_t countEven(const std::vector<Polygon>& data);
  size_t countOdd(const std::vector<Polygon>& data);
  size_t countVertices(const std::vector<Polygon>& data, size_t givenSize);
  void count(const std::vector<Polygon>& data, std::istream& in, std::ostream& out);
  void rmecho(std::vector<Polygon>& data, std::istream& in, std::ostream& out);
  void inframe(const std::vector<Polygon>& data, std::istream& in, std::ostream& out);

  using commandMap = std::map<std::string, std::function<void(std::istream&, std::ostream&)>>;
  commandMap mapCommandHandlers(std::vector<Polygon>& data);
}

#endif
