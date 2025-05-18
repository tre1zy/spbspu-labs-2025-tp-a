#include "commands.hpp"

void maslevtsov::get_area(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  polygons.empty();
  std::string subcommand;
  in >> subcommand;
  out << "AREA " << subcommand << '\n';
}

void maslevtsov::get_max(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  polygons.empty();
  std::string subcommand;
  in >> subcommand;
  out << "MAX " << subcommand << '\n';
}

void maslevtsov::get_min(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  polygons.empty();
  std::string subcommand;
  in >> subcommand;
  out << "MIN " << subcommand << '\n';
}

void maslevtsov::count_vertexes(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  polygons.empty();
  std::string subcommand;
  in >> subcommand;
  out << "COUNT " << subcommand << '\n';
}

void maslevtsov::echo(std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  polygons.empty();
  std::string subcommand;
  in >> subcommand;
  out << "ECHO " << subcommand << '\n';
}

void maslevtsov::remove_echo(std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  polygons.empty();
  std::string subcommand;
  in >> subcommand;
  out << "RMECHO " << subcommand << '\n';
}
