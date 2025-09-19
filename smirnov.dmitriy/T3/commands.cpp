#include "commands.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <map>
#include "polygon.hpp"

namespace smirnov
{
  using namespace std::placeholders;

  template< class UnaryPredicate >
  double getSumArea(const std::vector< Polygon >& polygons, UnaryPredicate P);
  double doAreaEven(const std::vector< Polygon >& polygons);
  double doAreaOdd(const std::vector< Polygon >& polygons);
  double doAreaMean(const std::vector< Polygon >& polygons);
  double doAreaNum(const std::vector< Polygon >& polygons, size_t n);

  void doAreaCommand(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
  {
    std::string s;
    in >> s;
    double result = 0.0;
    std::map< std::string, std::function< double() > > subcommand;
    {
      subcommand["EVEN"] = std::bind(doAreaEven, polygons);
      subcommand["ODD"] = std::bind(doAreaOdd, polygons);
      subcommand["MEAN"] = std::bind(doAreaMean, polygons);
    }
    try
    {
      size_t n = std::stoull(s);
      if (n < 3)
      {
        throw std::logic_error("FEW VERTEXES");
      }
      result = doAreaNum(polygons, n);
    }
    catch (const std::invalid_argument&)
    {
      result = subcommand[s]();
    }
    out << std::fixed << std::setprecision(1) << result << "\n";
  }

  void doMinMaxCommand(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out, const std::string& name)
  {
    std::string s;
    in >> s;
    if (polygons.empty())
    {
      throw std::logic_error("zero polygons");
    }
    if (s == "AREA")
    {
      if (name == "max")
      {
        out << std::fixed << std::setprecision(1) << getArea(*std::max_element(polygons.begin(), polygons.end(), minArea)) << "\n";
      }
      else
      {
        out << std::fixed << std::setprecision(1) << getArea(*std::min_element(polygons.begin(), polygons.end(), minArea)) << "\n";
      }
    }
    else if (s == "VERTEXES")
    {
      if (name == "max")
      {
        out << std::fixed << std::setprecision(1) << (*std::max_element(polygons.begin(), polygons.end(), minVertexes)).points.size() << "\n";
      }
      else
      {
        out << std::fixed << std::setprecision(1) << (*std::min_element(polygons.begin(), polygons.end(), minVertexes)).points.size() << "\n";
      }
    }
    else
    {
      throw std::logic_error("INVALID COMMAND");
    }
  }

  void doMaxCommand(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
  {
    doMinMaxCommand(polygons, in, out, "max");
  }

  void doMinCommand(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
  {
    doMinMaxCommand(polygons, in, out, "min");
  }

  void doCountCommand(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
  {
    std::string s;
    in >> s;
    if (s == "EVEN")
    {
      out << std::count_if(polygons.begin(), polygons.end(), isEvenCountVertexes) << "\n";
    }
    else if (s == "ODD")
    {
      out << std::count_if(polygons.begin(), polygons.end(),isOddCountVertexes) << "\n";
    }
    else if (s == std::to_string(std::stoi(s)))
    {
      size_t n = static_cast< size_t >(std::stoi(s));
      if (n < 3)
      {
        throw std::logic_error("FEW VERTEXES");
      }
      else
      {
        out << std::count_if(polygons.begin(), polygons.end(), std::bind(isNCountVertexes, _1, n)) << "\n";
      }
    }
    else
    {
      throw std::logic_error("INVALID COMMAND");
    }
  }

  void doRectsCommand(const std::vector< Polygon >& polygons, std::ostream& out)
  {
    out << std::count_if(polygons.begin(), polygons.end(), isRect) << "\n";
  }

  struct PolygonMaxSeq
  {
    size_t cur;
    size_t maxseq;
    bool operator()(const Polygon& polygon, const Polygon& data);
  };

  void doMaxseqCommand(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
  {
    smirnov::Polygon data;
    in >> data;
    if (data.points.size() < 3)
    {
      throw std::logic_error("INVALID COMMAND");
    }
    else
    {
      PolygonMaxSeq seq{0,0};
      size_t numberSeq = std::count_if(polygons.begin(), polygons.end(), std::bind(std::ref(seq), _1, data));
      if (numberSeq < 1)
      {
        throw std::logic_error("INVALID COMMAND");
      }
      else
      {
        out << seq.maxseq << "\n";
      }
    }
  }

  bool onSegment(const smirnov::Point& p, const smirnov::Point& q, const smirnov::Point& r) {
    return q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
           q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y);
  }

  int orientation(const smirnov::Point& p, const smirnov::Point& q, const smirnov::Point& r) {
    long long val = static_cast<long long>(q.y - p.y) * (r.x - q.x) -
                    static_cast<long long>(q.x - p.x) * (r.y - q.y);
    if (val == 0) return 0;
    return (val > 0) ? 1 : 2;
  }

  bool doIntersect(const smirnov::Point& p1, const smirnov::Point& q1,
                   const smirnov::Point& p2, const smirnov::Point& q2) {
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    if (o1 != o2 && o3 != o4) return true;
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false;
  }

  bool pointOnEdge(const Polygon& poly, const smirnov::Point& p)
  {
    for (size_t i = 0; i < poly.points.size(); ++i) {
      smirnov::Point a = poly.points[i];
      smirnov::Point b = poly.points[(i + 1) % poly.points.size()];

      long long cross = (p.y - a.y) * (b.x - a.x) - (p.x - a.x) * (b.y - a.y);
      if (cross == 0 &&
          std::min(a.x, b.x) <= p.x && p.x <= std::max(a.x, b.x) &&
          std::min(a.y, b.y) <= p.y && p.y <= std::max(a.y, b.y)) {
        return true;
      }
    }
    return false;
  }

  bool isPointInside(const Polygon& poly, const smirnov::Point& p)
  {
    if (pointOnEdge(poly, p)) {
      return true;
    }
    bool inside = false;
    for (size_t i = 0, j = poly.points.size() - 1; i < poly.points.size(); j = i++) {
      const smirnov::Point& pi = poly.points[i];
      const smirnov::Point& pj = poly.points[j];
      bool intersect = ((pi.y > p.y) != (pj.y > p.y)) &&
                       (p.x < (pj.x - pi.x) * (p.y - pi.y) / double(pj.y - pi.y) + pi.x);
      if (intersect)
        inside = !inside;
    }
    return inside;
  }

  bool hasIntersection(const Polygon& first, const Polygon& second)
  {
    for (size_t i = 0; i < first.points.size(); ++i) {
      smirnov::Point a1 = first.points[i];
      smirnov::Point a2 = first.points[(i + 1) % first.points.size()];
      for (size_t j = 0; j < second.points.size(); ++j) {
        smirnov::Point b1 = second.points[j];
        smirnov::Point b2 = second.points[(j + 1) % second.points.size()];
        if (doIntersect(a1, a2, b1, b2)) {
          return true;
        }
      }
    }
    for (const auto& p1 : first.points) {
      for (const auto& p2 : second.points) {
        if (p1.x == p2.x && p1.y == p2.y) {
          return true;
        }
      }
    }
    for (const auto& p : second.points) {
      if (isPointInside(first, p)) return true;
    }
    for (const auto& p : first.points) {
      if (isPointInside(second, p)) return true;
    }
    return false;
  }

  void doIntersections(const std::vector< Polygon > & data, std::istream & in, std::ostream & out)
  {
    Polygon polygon;
    in >> polygon;
    if (!in)
    {
      throw std::logic_error("Wrong argument");
    }
    auto isIntersected = std::bind(hasIntersection, std::cref(polygon), _1);
    out << std::count_if(data.begin(), data.end(), isIntersected) << '\n';
  }

  bool PolygonMaxSeq::operator()(const Polygon& polygon, const Polygon& data)
  {
    if (polygon == data)
    {
      cur++;
      maxseq = std::max(maxseq, cur);
    }
    else
    {
      cur = 0;
    }
    return maxseq;
  }

  template< class UnaryPredicate >
  double getSumArea(const std::vector< Polygon >& polygons, UnaryPredicate P)
  {
    std::vector< Polygon > rightPolygons;
    std::copy_if(polygons.cbegin(), polygons.cend(), std::back_inserter(rightPolygons), P);
    std::vector< double > areas;
    std::transform(rightPolygons.cbegin(), rightPolygons.cend(), std::back_inserter(areas), getArea);
    double result = std::accumulate(areas.cbegin(), areas.cend(), 0.0);
    return result;
  }

  double doAreaEven(const std::vector< Polygon >& polygons)
  {
    return getSumArea(polygons, isEvenCountVertexes);
  }

  double doAreaOdd(const std::vector< Polygon >& polygons)
  {
    return getSumArea(polygons, isOddCountVertexes);
  }

  double doAreaMean(const std::vector< Polygon >& polygons)
  {
    if (polygons.empty())
    {
      throw std::logic_error("NO POLYGONS FOR AREA MEAN COMMAND");
    }
    std::vector< double > areas;
    std::transform(polygons.cbegin(), polygons.cend(), std::back_inserter(areas), getArea);
    double result = std::accumulate(areas.cbegin(), areas.cend(), 0.0);
    result /= polygons.size();
    return result;
  }

  double doAreaNum(const std::vector< Polygon >& polygons, size_t n)
  {
    return getSumArea(polygons, std::bind(isNCountVertexes, _1, n));
  }
}

