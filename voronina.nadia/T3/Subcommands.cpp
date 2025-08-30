#include "Subcommands.h"

#include <iomanip>
#include <IOFmtguard.h>

double voronina::getAreaOfPolygon(const Polygon &shape)
{
  std::vector< double > areaOfTriangles(shape.points.size() - 2);

  auto shapePBegin = shape.points.cbegin();
  auto shapePEnd = shape.points.cend();
  auto bindForAriaTriangle = std::bind(getAreaOfTriangle, std::cref(shape.points[0]), _1, _2);

  std::transform(shapePBegin + 2, shapePEnd, shapePBegin + 1, areaOfTriangles.begin(), bindForAriaTriangle);

  return std::accumulate(areaOfTriangles.cbegin(), areaOfTriangles.cend(), 0.0);
}

double voronina::getAreaOfTriangle(const Point &p1, const Point &p2, const Point &p3)
{
  return 0.5 * std::abs((p1.x - p3.x) * (p2.y - p1.y) - (p1.x - p2.x) * (p3.y - p1.y));
}

double voronina::getSumArea(double areaSum, const Polygon &shape)
{
  return areaSum + getAreaOfPolygon(shape);
}

bool voronina::isDigitBool(char c)
{
  return static_cast< bool >(isdigit(c));
}

int voronina::isEqual(const Polygon &left, const Polygon &right)
{
  return left == right;
}

int voronina::getMaxSeq(int left, int right)
{
  if (left == 0 || right == 0)
  {
    return left;
  }
  return left + right;
}

bool voronina::areaComporator(const Polygon &left, const Polygon &right)
{
  return getAreaOfPolygon(left) < getAreaOfPolygon(right);
}

bool voronina::vertexesComporator(const Polygon &left, const Polygon &right)
{
  return left.points.size() < right.points.size();
}

bool voronina::isEven(const Polygon &shape)
{
  return shape.points.size() % 2 == 0;
}

bool voronina::isOdd(const Polygon &shape)
{
  return shape.points.size() % 2 != 0;
}

bool voronina::hasGivenAmountOfVertexes(const Polygon &shape, int vertexes)
{
  return shape.points.size() == static_cast< size_t >(vertexes);
}

bool voronina::isRightAngle(const Point &side1, const Point &side2)
{
  return (side1.x * side2.x + side1.y * side2.y) == 0;
}

bool voronina::isThereRightAngleInPolygon(const Polygon &shape)
{
  std::vector< Point > sides(shape.points.size());
  auto shapePBegin = shape.points.cbegin();
  std::transform(shapePBegin + 1, shape.points.cend(), shapePBegin, sides.begin(), getSide);
  sides[shape.points.size() - 1] = getSide(shape.points[0], shape.points[shape.points.size() - 1]);

  if (std::adjacent_find(sides.begin(), sides.end(), isRightAngle) != sides.end()) {
    return true;
  }

  return isRightAngle(sides.back(), sides.front());
}
voronina::Point voronina::getSide(const Point &p1, const Point &p2)
{
  Point newSide;
  newSide = { p1.x - p2.x, p1.y - p2.y };
  return newSide;
}

bool voronina::isTrue(bool element)
{
  return element != 0;
}

void voronina::getEven(const std::vector< Polygon > &shapes, std::ostream &out)
{
  std::vector< Polygon > evenPolygons;
  std::copy_if(shapes.begin(), shapes.end(), std::back_inserter(evenPolygons), isEven);
  std::vector< double > areas;
  std::transform(evenPolygons.cbegin(), evenPolygons.cend(), std::back_inserter(areas), getAreaOfPolygon);
  out << std::accumulate(areas.cbegin(), areas.cend(), 0.0);
}

void voronina::getOdd(const std::vector< Polygon > &shapes, std::ostream &out)
{
  std::vector< Polygon > oddPolygons;
  std::copy_if(shapes.begin(), shapes.end(), std::back_inserter(oddPolygons), isOdd);
  std::vector< double > areas;
  std::transform(oddPolygons.cbegin(), oddPolygons.cend(), std::back_inserter(areas), getAreaOfPolygon);
  out << std::accumulate(areas.cbegin(), areas.cend(), 0.0);
}

void voronina::getMean(const std::vector< Polygon > &shapes, std::ostream &out)
{
  if (shapes.size() == 0)
  {
    throw std::invalid_argument("ERROR: No shapes");
  }
  out << std::accumulate(shapes.cbegin(), shapes.cend(), 0.0, getSumArea) / shapes.size();
}

void voronina::getVertexes(const std::vector< Polygon > &shapes, std::ostream &out, int vertexes)
{
  const int MIN_AMOUNT_OF_VERTEXES = 3;
  if (vertexes >= MIN_AMOUNT_OF_VERTEXES)
  {
    using namespace std::placeholders;
    std::vector< Polygon > filtered;
    std::copy_if(shapes.cbegin(), shapes.cend(), std::back_inserter(filtered), std::bind(hasGivenAmountOfVertexes, _1, vertexes));
    std::vector< double > areas;
    std::transform(filtered.cbegin(), filtered.cend(), std::back_inserter(areas), getAreaOfPolygon);
    out << std::accumulate(areas.cbegin(), areas.cend(), 0.0);
  }
  else
  {
    throw std::invalid_argument("ERROR: Invalid amount of vertexes");
  }
}

void voronina::getAreaMax(const std::vector< Polygon > &shapes, std::ostream &out)
{
  iofmtguard ofmtguard(out);
  out << std::fixed << std::setprecision(1);
  out << getAreaOfPolygon(*std::max_element(shapes.cbegin(), shapes.cend(), areaComporator));
}

void voronina::getVertexesMax(const std::vector< Polygon > &shapes, std::ostream &out)
{
  out << (*std::max_element(shapes.cbegin(), shapes.cend(), vertexesComporator)).points.size();
}

void voronina::getAreaMin(const std::vector< Polygon > &shapes, std::ostream &out)
{
  iofmtguard ofmtguard(out);
  out << std::fixed << std::setprecision(1);
  out << getAreaOfPolygon(*std::min_element(shapes.cbegin(), shapes.cend(), areaComporator));
}

void voronina::getVertexesMin(const std::vector< Polygon > &shapes, std::ostream &out)
{
  out << (*std::min_element(shapes.cbegin(), shapes.cend(), vertexesComporator)).points.size();
}

void voronina::getEvenCount(const std::vector< Polygon > &shapes, std::ostream &out)
{
  out << std::count_if(shapes.cbegin(), shapes.cend(), isEven);
}

void voronina::getOddCount(const std::vector< Polygon > &shapes, std::ostream &out)
{
  out << std::count_if(shapes.cbegin(), shapes.cend(), isOdd);
}

void voronina::getVertexesCount(const std::vector< Polygon > &shapes, std::ostream &out, int vertexes)
{
  const int MIN_AMOUNT_OF_VERTEXES = 3;
  if (vertexes >= MIN_AMOUNT_OF_VERTEXES)
  {
    out << std::count_if(shapes.cbegin(), shapes.cend(), std::bind(hasGivenAmountOfVertexes, _1, vertexes));
  }
  else
  {
    throw std::invalid_argument("ERROR: Invalid amount of vertexes");
  }
}
