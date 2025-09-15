#include "commands.hpp"
#include <algorithm>
#include <functional>
#include <map>
#include <numeric>
#include "exceptions.hpp"
#include "polygon.hpp"
#include "subcommands.hpp"

using VecPolygons = std::vector< holodilov::Polygon >;

bool compareAreaWithTarget(const holodilov::Polygon& polygon, double targetArea)
{
  std::cout << "Target area = " << targetArea << '\n';
  std::cout << "polygon area = " << polygon.getArea() << "\n";
  return polygon.getArea() < targetArea;
}

VecPolygons accumulateEcho(VecPolygons vecPolygons, const holodilov::Polygon& polygon, const holodilov::Polygon& targetPolygon)
{
  vecPolygons.push_back(polygon);
  if (polygon == targetPolygon)
  {
    vecPolygons.push_back(polygon);
  }
  return vecPolygons;
}

void holodilov::commands::area(std::istream& is, std::ostream& os, const VecPolygons& vecPolygons)
{

}

void holodilov::commands::min(std::istream& is, std::ostream& os, const VecPolygons& vecPolygons)
{

}

void holodilov::commands::max(std::istream& is, std::ostream& os, const VecPolygons& vecPolygons)
{
  std::string subcommand;
  is >> subcommand;

  std::map< std::string, std::function< void() > > cmds;
  cmds["AREA"] = std::bind(subcommands::maxArea, std::ref(is), std::ref(os), std::cref(vecPolygons));
  cmds["VERTEXES"] = std::bind(subcommands::maxVertexes, std::ref(is), std::ref(os), std::cref(vecPolygons));

  try
  {
    cmds.at(subcommand)();
  }
  catch (const std::out_of_range& e)
  {
    throw InvalidCommandException();
  }
}

void holodilov::commands::count(std::istream& is, std::ostream& os, const VecPolygons& vecPolygons)
{
  std::string subcommand;
  is >> subcommand;
  try
  {
    int amountVertexes = std::stoi(subcommand);
    if (amountVertexes <= 0)
    {
      throw InvalidCommandException();
    }
    subcommands::countAmount(is, os, vecPolygons, amountVertexes);
  }
  catch (const std::invalid_argument& e)
  {
    std::map< std::string, std::function< void() > > cmds;
    cmds["EVEN"] = std::bind(subcommands::countEven, std::ref(is), std::ref(os), std::cref(vecPolygons));
    cmds["ODD"] = std::bind(subcommands::countOdd, std::ref(is), std::ref(os), std::cref(vecPolygons));
    try
    {
      cmds.at(subcommand)();
    }
    catch (const std::out_of_range& e)
    {
      throw InvalidCommandException();
    }
  }
}

void holodilov::commands::echo(std::istream& is, std::ostream& os, VecPolygons& vecPolygons)
{
  Polygon targetPolygon;
  is >> targetPolygon;
  if (!is || is.peek() != '\n')
  {
    throw InvalidCommandException();
  }

  const size_t amountDuplicates = std::count(vecPolygons.begin(), vecPolygons.end(), targetPolygon);

  auto accumulateBound = std::bind(accumulateEcho, std::placeholders::_1, std::placeholders::_2, std::cref(targetPolygon));
  vecPolygons = std::accumulate(vecPolygons.begin(), vecPolygons.end(), VecPolygons(0), accumulateBound);
  os << amountDuplicates << '\n';
}

void holodilov::commands::lessArea(std::istream& is, std::ostream& os, const VecPolygons& vecPolygons)
{
  Polygon targetPolygon;
  is >> targetPolygon;
  if (!is || is.peek() != '\n')
  {
    throw InvalidCommandException();
  }

  auto areaComparator = std::bind(compareAreaWithTarget, std::placeholders::_1, targetPolygon.getArea());
  size_t result = std::count_if(vecPolygons.begin(), vecPolygons.end(), areaComparator);
  os << result << '\n';
}