#include "commands.hpp"
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <functional>
#include "geometry.hpp"

namespace
{
  bool isEvenVrts(const demehin::Polygon& plg)
  {
    return plg.points.size() % 2 == 0;
  }

  bool isOddVrts(const demehin::Polygon& plg)
  {
    return !(isEvenVrts(plg));
  }

  struct VrtCntCheck
  {
    size_t cnt;

    bool operator()(const demehin::Polygon& plg) const
    {
      return plg.points.size() == cnt;
    }
  };

  bool compareVrtCnt(const demehin::Polygon& plg1, const demehin::Polygon& plg2)
  {
    return plg1.points.size() < plg2.points.size();
  }

  bool compareArea(const demehin::Polygon& plg1, const demehin::Polygon& plg2)
  {
    return getPlgArea(plg1) < getPlgArea(plg2);
  }
}

void demehin::printAreaSum(std::istream& in, const std::vector< Polygon >& plgs, std::ostream& out)
{
  std::string subcommand;
  in >> subcommand;
  std::vector< Polygon > tmp;
  if (subcommand == "EVEN")
  {
    std::copy_if(plgs.cbegin(), plgs.cend(), std::back_inserter(tmp), isEvenVrts);
  }
  else if (subcommand == "ODD")
  {
    std::copy_if(plgs.cbegin(), plgs.cend(), std::back_inserter(tmp), isOddVrts);
  }
  else if (subcommand == "MEAN")
  {
    if (plgs.size() == 0)
    {
      throw std::invalid_argument("not enough figures");
    }
    tmp = plgs;
  }
  else
  {
    size_t vrt_cnt = std::stoull(subcommand);
    if (vrt_cnt < 3)
    {
      throw std::invalid_argument("not enough vertexes");
    }

    VrtCntCheck check{ vrt_cnt };
    std::copy_if(plgs.cbegin(), plgs.cend(), std::back_inserter(tmp), check);
  }

  std::vector< double > areas;
  std::transform(tmp.begin(), tmp.end(), std::back_inserter(areas), getPlgArea);
  double res = std::accumulate(areas.begin(), areas.end(), 0);
  if (subcommand == "MEAN")
  {
    res /= plgs.size();
  }
  out << std::setprecision(1) << std::fixed << res;
}

void demehin::printMaxValueOf(std::istream& in, const std::vector< Polygon >& plgs, std::ostream& out)
{
  std::string subcommand;
  in >> subcommand;
  if (subcommand == "AREA")
  {
    auto res = (*std::max_element(plgs.cbegin(), plgs.cend(), compareArea));
    out << getPlgArea(res);
  }
  else if (subcommand == "VERTEXES")
  {
    auto res = (*std::max_element(plgs.cbegin(), plgs.cend(), compareVrtCnt));
    out << res.points.size();
  }
  else
  {
    throw std::invalid_argument("unknown command");
  }
}

void demehin::printMinValueOf(std::istream& in, const std::vector< Polygon >& plgs, std::ostream& out)
{
  std::string subcommand;
  in >> subcommand;
  if (subcommand == "AREA")
  {
    auto res = (*std::min_element(plgs.cbegin(), plgs.cend(), compareArea));
    out << std::setprecision(1) << std::fixed << getPlgArea(res);
  }
  else if (subcommand == "VERTEXES")
  {
    auto res = (*std::min_element(plgs.cbegin(), plgs.cend(), compareVrtCnt));
    out << std::setprecision(1) << std::fixed << res.points.size();
  }
  else
  {
    throw std::invalid_argument("unknown command");
  }
}
