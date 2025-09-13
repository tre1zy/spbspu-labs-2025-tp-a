#ifndef COMMANDS_H
#define COMMANDS_H
#include <vector>
#include <istream>
#include <ostream>
#include "geom.hpp"
namespace nikonov
{
  void getArea(const std::vector< Polygon >&, std::istream& in, std::ostream& out);
  namespace area
  {
    void getEvenArea(const std::vector< Polygon >& data, std::ostream& out);
    void getOddArea(const std::vector< Polygon >& data, std::ostream& out);
    void getMeanArea(const std::vector< Polygon >& data, std::ostream& out);
    void getVertexCountArea(const std::vector< Polygon >& data, std::size_t, std::ostream& out);
  }

  void getMax(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  namespace max
  {
    void getArea(const std::vector< Polygon >& data, std::ostream& out);
    void getVertexes(const std::vector< Polygon >& data, std::ostream& out);
  }

  void getMin(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  namespace min
  {
    void getArea(const std::vector< Polygon >& data, std::ostream& out);
    void getVertexes(const std::vector< Polygon >& data, std::ostream& out);
  }

  void getCount(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  namespace count
  {
    void getEven(const std::vector< Polygon >& data, std::ostream& out);
    void getOdd(const std::vector< Polygon >& data, std::ostream& out);
    void getVertexCount(const std::vector< Polygon >& data, std::size_t, std::ostream& out);
  }
  void getPerms(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);

  void getMaxseq(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
}
#endif
