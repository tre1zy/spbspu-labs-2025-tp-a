#include "commands.hpp"
#include <iterator>
#include <algorithm>
#include <numeric>

namespace
{
  double calcPoints(const mozhegova::Point & p1, const mozhegova::Point & p2)
  {
    return p1.x * p2.y - p1.y * p2.x;
  }

  double getArea(const mozhegova::Polygon & poly)
  {
    mozhegova::Polygon rotPoly(poly);
    std::rotate(rotPoly.points.begin(), rotPoly.points.begin() + 1, rotPoly.points.end());
    auto it1 = poly.points.begin();
    auto it2 = poly.points.end();
    auto it3 = rotPoly.points.begin();
    double sum = std::inner_product(it1, it2, it3, 0.0, std::plus< double >(), calcPoints);
    return std::abs(sum) * 0.5;
  }
}

void mozhegova::printArea(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  
}
void mozhegova::printMax(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
void mozhegova::printMin(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
void mozhegova::printCount(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
void mozhegova::printEcho(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
void mozhegova::printSame(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons);
