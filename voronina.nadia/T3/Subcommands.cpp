#include "Subcommands.h"

namespace voronina
{
  double getAreaOfPolygon(const Polygon& shape)
  {
    std::vector< double > areaOfTriangles(shape.points.size() - 2);
    std::transform(
      shape.points.cbegin() + 2, shape.points.cend(),
      shape.points.cbegin() + 1, areaOfTriangles.begin(),
      std::bind(getAreaOfTriangle, std::cref(shape.points[0]), std::placeholders::_1, std::placeholders::_2)
    );
      return std::accumulate(areaOfTriangles.cbegin(), areaOfTriangles.cend(), 0.0);
  }
  double getAreaOfTriangle(const Point& p1, const Point& p2, const Point& p3)
  {
    return 0.5 * std::abs((p1.x - p3.x) * (p2.y - p1.y) - (p1.x - p2.x) * (p3.y - p1.y));
  }
  double getAreaForEvenPolygons(double areaSum, const Polygon& shape)
  {
    if (shape.points.size() % 2 == 0)
    {
      return areaSum + getAreaOfPolygon(shape);
    }
    return areaSum;
  }
  double getAreaForOddPolygons(double areaSum, const Polygon& shape)
  {
    if (shape.points.size() % 2 != 0)
    {
      return areaSum + getAreaOfPolygon(shape);
    }
    return areaSum;
  }
  double getSumArea(double areaSum, const Polygon& shape)
  {
    return areaSum + getAreaOfPolygon(shape);
  }
  bool isDigitBool(char c)
  {
    return static_cast< bool >(isdigit(c));
  }

  double getAreaOfGivenAmountOfVertexes(double areaSum, const Polygon &shape, int vertexes)
  {
    if (shape.points.size() == vertexes)
    {
      return areaSum + getAreaOfPolygon(shape);
    }
    return areaSum;
  }
  int isEqual(const Polygon &left, const Polygon &right)
  {
    return left == right;
  }
  int getMaxSeq(int left, int right)
  {
    if (left == 0 || right == 0)
    {
      return left;
    }
    return left + right;
  }
  bool areaComporator(const Polygon &left, const Polygon &right)
  {
    return getAreaOfPolygon(left) < getAreaOfPolygon(right);
  }
  bool vertexesComporator(const Polygon &left, const Polygon &right)
  {
    return left.points.size() < right.points.size();
  }
  bool isEven(const Polygon &shape)
  {
    return shape.points.size() % 2 == 0;
  }
  bool isOdd(const Polygon &shape)
  {
    return shape.points.size() % 2 != 0;
  }
  bool hasGivenAmountOfVertexes(const Polygon &shape, int vertexes)
  {
    return shape.points.size() == vertexes;
  }
  bool isRightAngle(const Point &side1, const Point &side2)
  {
    return (side1.x * side2.x + side1.y * side2.y) == 0;
  }
  bool isThereRightAngleInPolygon(const Polygon &shape)
  {
    std::vector<Point> sides(shape.points.size());
    std::transform(
      shape.points.cbegin() + 1, shape.points.cend(),
      shape.points.cbegin(), sides.begin(),
      getSide);
    sides[shape.points.size() - 1] = getSide(shape.points[0],
      shape.points[shape.points.size() - 1]);

    std::vector<bool> hasRightAngleVector(shape.points.size());
    std::transform(
      sides.cbegin() + 1, sides.cend(),
      sides.cbegin(), hasRightAngleVector.begin(),
      isRightAngle);
    hasRightAngleVector[sides.size() - 1] = isRightAngle(sides[0],
      sides[sides.size() - 1]);
    return std::any_of(
      hasRightAngleVector.cbegin(), hasRightAngleVector.cend(), isTrue);
  }
  Point getSide(const Point &p1, const Point &p2)
  {
    Point newSide;
    newSide.x = p1.x - p2.x;
    newSide.y = p1.y - p2.y;
    return newSide;
  }
  bool isTrue(bool element)
  {
    return element != 0;
  }
}

