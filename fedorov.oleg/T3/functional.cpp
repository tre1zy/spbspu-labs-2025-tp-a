#include <numeric>
#include <sstream>
#include <iterator>
#include <stdexcept>
#include <cctype>
#include <algorithm>
#include "functional.hpp"
#include "polygon.hpp"

namespace fedorov
{
  double calcPolygonArea(const Polygon &poly)
  {
    if (poly.points.size() < 3)
    {
      return 0.0;
    }

    struct AreaAccumulator
    {
      double operator()(const Point &p1, const Point &p2) const
      {
        return p1.x * p2.y - p2.x * p1.y;
      }
    };

    double sum = 0.0;
    const Point &first = poly.points.front();
    const Point &last = poly.points.back();

    if (poly.points.size() > 1)
    {
      sum = std::inner_product(poly.points.begin(), poly.points.end() - 1, poly.points.begin() + 1, 0.0, std::plus<>(),
                               AreaAccumulator());
    }

    sum += (last.x * first.y - first.x * last.y);

    return std::abs(sum) / 2.0;
  }

  bool LineValidator::operator()(const Line &line) const
  {
    if (line.data.empty())
    {
      return false;
    }
    std::istringstream iss(line.data);
    size_t num;
    return static_cast< bool >(iss >> num);
  }

  Polygon LineToPolygonConverter::operator()(const Line &line) const
  {
    std::istringstream iss(line.data);
    Polygon poly;
    if (!(iss >> poly))
    {
      return Polygon();
    }

    std::string remaining;
    if (iss >> remaining)
    {
      return Polygon();
    }

    return poly;
  }

  bool PolygonValidator::operator()(const Polygon &poly) const
  {
    return poly.points.size() >= 3;
  }

  void readPolygons(std::istream &in, std::vector< Polygon > &polygons)
  {
    polygons.clear();

    std::vector< Line > lines;
    std::copy(std::istream_iterator< Line >(in), std::istream_iterator< Line >(), std::back_inserter(lines));

    std::vector< Line > validLines;
    std::copy_if(lines.begin(), lines.end(), std::back_inserter(validLines), LineValidator());

    std::vector< Polygon > allPolygons;
    std::transform(validLines.begin(), validLines.end(), std::back_inserter(allPolygons), LineToPolygonConverter());

    std::copy_if(allPolygons.begin(), allPolygons.end(), std::back_inserter(polygons), PolygonValidator());
  }

  bool AreaComparator::operator()(const Polygon &p1, const Polygon &p2) const
  {
    return calcPolygonArea(p1) < calcPolygonArea(p2);
  }

  bool VerticesComparator::operator()(const Polygon &p1, const Polygon &p2) const
  {
    return p1.points.size() < p2.points.size();
  }

  size_t countIf(const std::vector< Polygon > &polys, const LessAreaPredicate &pred)
  {
    return std::count_if(polys.begin(), polys.end(), pred);
  }

  EchoHandler::EchoHandler(const Polygon &ref):
    reference(ref),
    count(0)
  {}

  void EchoHandler::operator()(std::vector< Polygon > &result, const Polygon &poly) const
  {
    result.push_back(poly);
    if (poly == reference)
    {
      result.push_back(poly);
      ++count;
    }
  }

  void processEcho(std::vector< Polygon > &polys, const Polygon &ref)
  {
    std::vector< Polygon > temp;
    temp.reserve(polys.size() * 2);
    EchoHandler handler(ref);
    std::for_each(polys.begin(), polys.end(),
                  std::bind(&EchoHandler::operator(), &handler, std::ref(temp), std::placeholders::_1));
    polys.swap(temp);
  }

  AreaAccumulator::AreaAccumulator(const std::string &param):
    param(param)
  {}

  double AreaAccumulator::operator()(double acc, const Polygon &poly) const
  {
    const size_t vertices = poly.points.size();
    if (param == "ODD" && vertices % 2 != 0)
    {
      return acc + calcPolygonArea(poly);
    }
    if (param == "EVEN" && vertices % 2 == 0)
    {
      return acc + calcPolygonArea(poly);
    }
    if (param == "MEAN")
    {
      return acc + calcPolygonArea(poly);
    }

    try
    {
      size_t num = std::stoul(param);
      if (num == vertices)
      {
        return acc + calcPolygonArea(poly);
      }
    }
    catch (...)
    {}

    return acc;
  }

  CountAccumulator::CountAccumulator(const std::string &param):
    param(param)
  {}

  size_t CountAccumulator::operator()(size_t cnt, const Polygon &poly) const
  {
    const size_t vertices = poly.points.size();
    if (param == "ODD" && vertices % 2 != 0)
    {
      return cnt + 1;
    }
    if (param == "EVEN" && vertices % 2 == 0)
    {
      return cnt + 1;
    }

    try
    {
      size_t num = std::stoul(param);
      if (num == vertices)
      {
        return cnt + 1;
      }
    }
    catch (...)
    {}

    return cnt;
  }

  LessAreaPredicate::LessAreaPredicate(double threshold):
    threshold(threshold)
  {}

  bool LessAreaPredicate::operator()(const Polygon &poly) const
  {
    return calcPolygonArea(poly) < threshold;
  }
}
