#include "commands.hpp"
#include <iomanip>
#include <numeric>
#include <algorithm>
#include <StreamGuard.hpp>
namespace
{
  struct IsEvenPredicate
  {
    bool operator()(const nikonov::Polygon& poly) const
    {
      return (poly.points.size() % 2) == 0;
    }
  };

  struct IsOddPredicate
  {
    bool operator()(const nikonov::Polygon& poly) const
    {
      return (poly.points.size() % 2) == 1;
    }
  };

  struct AnyPredicate
  {
    bool operator()(const nikonov::Polygon&) const
    {
      return true;
    }
  };

  struct HasVertexCountPredicate
  {
    size_t n;
    explicit HasVertexCountPredicate(std::size_t cnt):
      n(cnt)
    {}
    bool operator()(const nikonov::Polygon& poly) const
    {
      return poly.points.size() == n;
    }
  };

  double gaussArea(const nikonov::Point& a, const nikonov::Point& b)
  {
    return a.x * b.y - a.y * b.x;
  }

  double getPolygonArea(const nikonov::Polygon& poly)
  {
    const auto& pts  = poly.points;
    double sum = std::inner_product(pts.begin(), pts.end() - 1, pts.begin() + 1, 0.0, std::plus< double >{}, gaussArea);
    sum += gaussArea(pts.back(), pts.front());
    return std::abs(sum) / 2.0;
  }

  template < typename Pred >
  double calculateAreaIf(const std::vector< nikonov::Polygon >& data, Pred condition)
  {
    std::vector< nikonov::Polygon > tempPoly;
    std::copy_if(data.begin(), data.end(), std::back_inserter(tempPoly), condition);
    std::vector< double > areas;
    std::transform(tempPoly.begin(), tempPoly.end(), std::back_inserter(areas), getPolygonArea);
    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }
}

void nikonov::getArea(const std::vector< Polygon >& data, std::istream& in, std::ostream& out)
{
  std::string subcommand;
  in >> subcommand;
  StreamGuard guard(out);
  out << std::fixed << std::setprecision(1);
  if (subcommand == "EVEN")
  {
    out << calculateAreaIf(data, IsEvenPredicate());
  }
  else if (subcommand == "ODD")
  {
    out << calculateAreaIf(data, IsOddPredicate());
  }
  else if (subcommand == "MEAN")
  {
    if (data.empty())
    {
      throw std::logic_error("Mean cant be calculated!");
    }
    out << calculateAreaIf(data, AnyPredicate()) / data.size();
  }
  else
  {
    try
    {
      size_t cnt = std::stoul(subcommand);
      if (cnt < 3)
      {
        throw std::invalid_argument("At least 3 vertex needed!");
      }
      out << calculateAreaIf(data, HasVertexCountPredicate(cnt));
    }
    catch (const std::exception&)
    {
      out << "<INVALID COMMAND>\n";
    }
  }
}
