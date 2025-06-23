#include "commands.hpp"

namespace averenkov
{

  double AreaAccumulator::operator()(double sum, const Polygon& poly) const
  {
    return sum + calculateArea(poly);
  }

  double EvenAreaAccumulator::operator()(double sum, const Polygon& poly) const
  {
    return (poly.points.size() % 2 == 0) ? AreaAccumulator()(sum, poly) : sum;
  }

  double OddAreaAccumulator::operator()(double sum, const Polygon& poly) const
  {
    return (poly.points.size() % 2 != 0) ? AreaAccumulator()(sum, poly) : sum;
  }

  double NumVertexAreaAccumulator::operator()(double sum, const Polygon& poly) const
  {
    return (poly.points.size() == num) ? AreaAccumulator()(sum, poly) : sum;
  }

  bool AreaComparator::operator()(const Polygon& a, const Polygon& b) const
  {
    return calculateArea(a) < calculateArea(b);
  }

  bool VertexCountComparator::operator()(const Polygon& a, const Polygon& b) const
  {
    return a.points.size() < b.points.size();
  }

  bool EvenVertexCounter::operator()(const Polygon& poly) const
  {
    return poly.points.size() % 2 == 0;
  }

  bool OddVertexCounter::operator()(const Polygon& poly) const
  {
    return poly.points.size() % 2 != 0;
  }

  bool NumVertexCounter::operator()(const Polygon& poly) const
  {
    return poly.points.size() == num;
  }

  bool RightAngleChecker::operator()(const Polygon& poly) const
  {
    const auto& pts = poly.points;
    if (pts.size() < 3)
    {
      return false;
    }

    for (size_t i = 0; i < pts.size(); ++i)
    {
      size_t j = (i + 1) % pts.size();
      size_t k = (i + 2) % pts.size();

      int dx1 = pts[j].x - pts[i].x;
      int dy1 = pts[j].y - pts[i].y;
      int dx2 = pts[k].x - pts[j].x;
      int dy2 = pts[k].y - pts[j].y;

      if (dx1 * dx2 + dy1 * dy2 == 0)
      {
        return true;
      }
    }
    return false;
  }

  bool PolygonEqual::operator()(const Polygon& a, const Polygon& b) const
  {
    if (a == target && b == target)
    {
      return true;
    }
    return false;
  }

  double calculateArea(const Polygon& poly)
  {
    double area = 0.0;
    const auto& pts = poly.points;
    for (size_t i = 0; i < pts.size(); ++i)
    {
      size_t j = (i + 1) % pts.size();
      area += (pts[i].x * pts[j].y) - (pts[j].x * pts[i].y);
    }
    return std::abs(area) / 2.0;
  }

  void printAreaSum(std::istream& in, const std::vector< Polygon >& polygons, std::ostream& out)
  {
    std::string param;
    in >> param;

    if (param == "EVEN")
    {
      double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0, EvenAreaAccumulator());
      averenkov::iofmtguard guard(out);
      out << std::fixed << std::setprecision(1) << sum;
    }
    else if (param == "ODD")
    {
      double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0, OddAreaAccumulator());
      averenkov::iofmtguard guard(out);
      out << std::fixed << std::setprecision(1) << sum;
    }
    else if (param == "MEAN")
    {
      if (polygons.empty())
      {
        throw std::runtime_error("No polygons for MEAN calculation");
      }
      double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0, AreaAccumulator());
      averenkov::iofmtguard guard(out);
      out << std::fixed << std::setprecision(1) << (sum / polygons.size());
    }
    else
    {
      size_t num = std::stoul(param);
      double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0, NumVertexAreaAccumulator{ num });
      averenkov::iofmtguard guard(out);
      out << std::fixed << std::setprecision(1) << sum;
    }
  }

  void printMaxValueOf(std::istream& in, const std::vector< Polygon >& polygons, std::ostream& out)
  {
    if (polygons.empty())
    {
      throw std::runtime_error("No polygons for MAX calculation");
    }

    std::string param;
    in >> param;

    if (param == "AREA")
    {
      auto maxIt = std::max_element(polygons.begin(), polygons.end(), AreaComparator());
      averenkov::iofmtguard guard(out);
      out << std::fixed << std::setprecision(1) << calculateArea(*maxIt);
    }
    else if (param == "VERTEXES")
    {
      auto maxIt = std::max_element(polygons.begin(), polygons.end(), VertexCountComparator());
      out << maxIt->points.size();
    }
    else
    {
      throw std::runtime_error("Invalid MAX parameter");
    }
  }

  void printMinValueOf(std::istream& in, const std::vector< Polygon >& polygons, std::ostream& out)
  {
    if (polygons.empty())
    {
      throw std::runtime_error("No polygons for MIN calculation");
    }

    std::string param;
    in >> param;

    if (param == "AREA")
    {
      auto minIt = std::min_element(polygons.begin(), polygons.end(), AreaComparator());
      averenkov::iofmtguard guard(out);
      out << std::fixed << std::setprecision(1) << calculateArea(*minIt);
    }
    else if (param == "VERTEXES")
    {
      auto minIt = std::min_element(polygons.begin(), polygons.end(), VertexCountComparator());
      out << minIt->points.size();
    }
    else
    {
      throw std::runtime_error("Invalid MIN parameter");
    }
  }

  void printCountOf(std::istream& in, const std::vector< Polygon >& polygons, std::ostream& out)
  {
    std::string param;
    if (param == "EVEN")
    {
      size_t count = std::count_if(polygons.begin(), polygons.end(), EvenVertexCounter());
      out << count;
    }
    else if (param == "ODD")
    {
      size_t count = std::count_if(polygons.begin(), polygons.end(), OddVertexCounter());
      out << count;
    }
    else
    {
      try
      {
        size_t num = std::stoul(param);
        size_t count = std::count_if(polygons.begin(), polygons.end(), NumVertexCounter{ num });
        out << count;
      }
      catch (...)
      {
        throw std::runtime_error("Invalid COUNT parameter");
      }
    }
  }

  void printPermsCnt(std::istream& in, const std::vector< Polygon >& polygons, std::ostream& out)
  {
    Polygon target;
    if (!(in >> target))
    {
      throw std::runtime_error("Invalid PERMS parameter");
    }

    size_t count = std::count_if(polygons.begin(), polygons.end(),
      std::bind(std::equal_to< Polygon >(), std::placeholders::_1, target));
    out << count;
  }

  void printRightsCnt(const std::vector< Polygon >& polygons, std::ostream& out)
  {
    size_t count = std::count_if(polygons.begin(), polygons.end(), RightAngleChecker());
    out << count;
  }

  void printRmEcho(std::istream& in, std::vector< Polygon >& polygons, std::ostream& out)
  {
    Polygon target;
    if (!(in >> target))
    {
      throw std::runtime_error("Invalid RMECHO parameter");
    }

    size_t removed = 0;
    auto newEnd = std::unique(polygons.begin(), polygons.end(), PolygonEqual{ target });

    removed = std::distance(newEnd, polygons.end());
    polygons.erase(newEnd, polygons.end());
    out << removed;
  }
}
