#include "command-helpers.hpp"

double belyaev::accumulateTerm(double sum, int i, const std::vector<Point>& pnts, int n)
{
  const int next_i = (i + 1) % n;
  return sum + (pnts[i].y + pnts[next_i].y) * (pnts[i].x - pnts[next_i].x);
}

double belyaev::calcArea(const Polygon& src)
{
  const int amount = src.points.size();
  std::vector<int> indices(amount);
  std::iota(indices.begin(), indices.end(), 0);

  using namespace std::placeholders;
  auto accumTermFunc = std::bind(accumulateTerm, _1, _2, std::cref(src.points), amount);
  double area = std::accumulate(indices.begin(), indices.end(), 0.0, accumTermFunc);
  return std::abs(area) / 2.0;
}

bool belyaev::compareAreas(const Polygon& lhs, const Polygon& rhs)
{
  return calcArea(lhs) < calcArea(rhs);
}

bool belyaev::compareVertices(const Polygon& lhs, const Polygon& rhs)
{
  return lhs.points.size() < rhs.points.size();
}

bool belyaev::isPolygonEven(const Polygon& src)
{
  return src.points.size() % 2 == 0;
}

bool belyaev::isPolygonOdd(const Polygon& src)
{
  return src.points.size() % 2 != 0;
}

bool belyaev::isPolygonOfSize(const Polygon& src, const size_t& userSize)
{
  return src.points.size() == userSize;
}

bool belyaev::rmEchoHelper(const Polygon& rmPolygon, const Polygon& lhs, const Polygon& rhs)
{
  return (rmPolygon == lhs) && (rmPolygon == rhs);
}

bool belyaev::isStringNumeric(const std::string& str)
{
  using namespace std::placeholders;
  auto isNumericBind = std::bind(isdigit, _1);
  return std::all_of(str.begin(), str.end(), isNumericBind);
}

size_t belyaev::getVertices(const Polygon& src)
{
  return src.points.size();
}

void belyaev::compareCoordsMin(const Point& pnt, int& minX, int& minY)
{
  if (pnt.x < minX)
  {
    minX = pnt.x;
  }
  if (pnt.y < minY)
  {
    minY = pnt.y;
  }
}

void belyaev::compareCoordsMax(const Point& pnt, int& maxX, int& maxY)
{
  if (pnt.x > maxX)
  {
    maxX = pnt.x;
  }
  if (pnt.y > maxY)
  {
    maxY = pnt.y;
  }
}

void belyaev::getMinMaxCoordsInPoly(const Polygon& src, int& minMaxX, int& minMaxY, std::string subCommand)
{
  using namespace std::placeholders;
  if (subCommand == "min")
  {
    auto compareCoordsBind = std::bind(compareCoordsMin, _1, std::ref(minMaxX), std::ref(minMaxY));
    std::for_each(src.points.begin(), src.points.end(), compareCoordsBind);
  }
  else if (subCommand == "max")
  {
    auto compareCoordsBind = std::bind(compareCoordsMax, _1, std::ref(minMaxX), std::ref(minMaxY));
    std::for_each(src.points.begin(), src.points.end(), compareCoordsBind);
  }
  else
  {
    throw std::logic_error("getMinMaxCoordsInPoly: INVALID SUBCOMMAND");
  }
}
