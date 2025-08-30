#include "commands.hpp"

void krylov::doAreaComm(const std::vector< Polygon >& polygons, std::ostream& out, std::istream& in)
{
  StreamGuard guard(out);
  std::string subcommand;
  in >> subcommand;
  std::map< std::string, std::function< void() > > commands;
  commands["EVEN"] = std::bind(printAreaEven, std::cref(polygons), std::ref(std::cout));
  commands["ODD"] = std::bind(printAreaOdd, std::cref(polygons), std::ref(std::cout));
  commands["MEAN"] = std::bind(printAreaMean, std::cref(polygons), std::ref(std::cout));
  commands["VERTEXES"] = std::bind(printAreaVertexes, std::cref(polygons), std::ref(std::cout), std::cref(s));
  out << std::fixed << std::setprecision(1);
  try
  {
    commands["VERTEXES"]();
  }
  catch (const std::exception &)
  {
    commands.at(subcommand)();
  }
}

void krylov::doMaxComm(const std::vector< Polygon>& polygons, std::ostream &out, std::istream& in)
{
  StreamGuard guard(out);
  if (polygons.size() < 1)
  {
    throw std::logic_error("No maximum\n");
  }
  std::string subcommand;
  in >> subcommand;
  std::map< std::string, std::function< void() > > commands;
  commands["AREA"] = std::bind(printMaxArea, std::cref(polygons), std::ref(std::cout));
  commands["VERTEXES"] = std::bind(printMaxVertexes, std::cref(polygons), std::ref(std::cout));
  commands.at(subcommand)();
}

void krylov::doMinComm(const std::vector< Polygon >& polygons, std::ostream& out, std::istream& in)
{
  StreamGuard guard(out);
  if (polygons.size() < 1)
  {
    throw std::logic_error("No minimum!\n");
  }
  std::string subcommand;
  in >> subcommand;
  std::map< std::string, std::function< void() > > commands;
  commands["AREA"] = std::bind(printMinArea, std::cref(polygons), std::ref(std::cout));
  commands["VERTEXES"] = std::bind(printMinVertexes, std::cref(polygons), std::ref(std::cout));
  commands.at(subcommand)();
}

void krylov::doCountComm(const std::vector< Polygon >& polygons, std::ostream& out, std::istream& in)
{
  StreamGuard guard(out);
  std::string subcommand;
  in >> subcommand;
  std::map< std::string, std::function< void() > > commands;
  commands["EVEN"] = std::bind(printCountEven, std::cref(polygons), std::ref(std::cout));
  commands["ODD"] = std::bind(printCountOdd, std::cref(polygons), std::ref(std::cout));
  commands["VERTEXES"] = std::bind(printCountVertexes, std::cref(polygons), std::ref(std::cout), std::cref(s));
  try
  {
    commands["VERTEXES"]();
  }
  catch (const std::exception&)
  {
    commands.at(subcommand)();
  }
}
