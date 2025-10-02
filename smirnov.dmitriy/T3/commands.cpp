#include "commands.hpp"
#include <algorithm>
#include <functional>
#include <iomanip>
#include <map>
#include <string>
#include <vector>
#include "polygon.hpp"
#include "scopeguard.hpp"

template < class UnaryPredicate >
double getSumArea(const std::vector< smirnov::Polygon >& polygons, UnaryPredicate P);
double doAreaEven(const std::vector< smirnov::Polygon >& polygons);
double doAreaOdd(const std::vector< smirnov::Polygon >& polygons);
double doAreaMean(const std::vector< smirnov::Polygon >& polygons);
double doAreaNum(const std::vector< smirnov::Polygon >& polygons, size_t n);

void smirnov::doAreaCommand(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  std::string s;
  in >> s;
  double result = 0.0;
  ScopeGuard guard(out);

  const std::map< std::string, std::function< double() > > subcommand =
  {
    { "EVEN", std::bind(doAreaEven, polygons) },
    { "ODD", std::bind(doAreaOdd, polygons) },
    { "MEAN", std::bind(doAreaMean, polygons) }
  };

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
    result = subcommand.at(s)();
  }
  out << std::fixed << std::setprecision(1) << result << "\n";
}

void doMinMaxCommand(const std::vector< smirnov::Polygon >& polygons, std::istream& in, std::ostream& out, const std::string& name)
{
  std::string s;
  in >> s;

  smirnov::ScopeGuard guard(out);

  if (polygons.empty())
  {
    throw std::logic_error("zero polygons");
  }

  out << std::fixed << std::setprecision(1);

  const std::map<std::string, std::map<std::string, std::function<void()>>> commands =
    {
    {
      "AREA",
      {
          {
            "max", [&]
              {
              auto it = std::max_element(polygons.begin(), polygons.end(), smirnov::minArea);
              if (it != polygons.end()) {
                out << smirnov::getArea(*it) << "\n";
              }
            }
          },
          {
            "min", [&]
              {
              auto it = std::min_element(polygons.begin(), polygons.end(), smirnov::minArea);
              if (it != polygons.end()) {
                out << smirnov::getArea(*it) << "\n";
              }
            }
          }
      }
    },
    {
      "VERTEXES", {
          {
            "max", [&]
              {
                auto it = std::max_element(polygons.begin(), polygons.end(), smirnov::minVertexes);
                if (it != polygons.end())
                {
                  out << it->points.size() << "\n";
              }
            }
          },
          {
            "min", [&]
            {
              auto it = std::min_element(polygons.begin(), polygons.end(), smirnov::minVertexes);
              if (it != polygons.end()) {
                out << it->points.size() << "\n";
            }
          }
        }
      }
    }
  };

  auto cmd = commands.find(s);
  if (cmd == commands.end())
  {
    throw std::logic_error("INVALID COMMAND");
  }

  auto subCmd = cmd->second.find(name);

  if (subCmd == cmd->second.end())
  {
    throw std::logic_error("INVALID COMMAND");
  }

  subCmd->second();
}

void smirnov::doMaxCommand(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  doMinMaxCommand(polygons, in, out, "max");
}

void smirnov::doMinCommand(const std::vector< smirnov::Polygon >& polygons, std::istream& in, std::ostream& out)
{
  doMinMaxCommand(polygons, in, out, "min");
}

void smirnov::doCountCommand(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  std::string s;
  in >> s;

  const std::map<std::string, std::function<size_t()>> commands =
  {
   {
      "EVEN", [&]
      {
        return std::count_if(polygons.begin(), polygons.end(), isEvenCountVertexes);
      }
    },
    {
      "ODD", [&]
      {
        return std::count_if(polygons.begin(), polygons.end(), isOddCountVertexes);
      }
    }
  };

  auto it = commands.find(s);
  if (it != commands.end())
  {
    out << it->second() << "\n";
    return;
  }

  if (std::all_of(s.cbegin(), s.cend(), ::isdigit))
  {
    size_t n = std::stoull(s);
    if (n < 3)
    {
      throw std::logic_error("FEW VERTEXES");
    }
    out << std::count_if(polygons.begin(), polygons.end(), std::bind(isNCountVertexes, std::placeholders::_1, n)) << "\n";
    return;
  }

  throw std::logic_error("INVALID COMMAND");
}

void smirnov::doRectsCommand(const std::vector< Polygon >& polygons, std::ostream& out)
{
  out << std::count_if(polygons.begin(), polygons.end(), isRect) << "\n";
}

struct PolygonMaxSeq
{
  size_t cur = 0;
  size_t maxseq = 0;
  bool operator()(const smirnov::Polygon& Polygon, const smirnov::Polygon& data);
};

bool PolygonMaxSeq::operator()(const smirnov::Polygon& Polygon, const smirnov::Polygon& data)
{
  if (Polygon == data)
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

void doMaxseqCommand(const std::vector< smirnov::Polygon >& polygons, std::istream& in, std::ostream& out)
{
  smirnov::Polygon data;
  in >> data;
  if (data.points.size() < 3)
  {
    throw std::logic_error("INVALID COMMAND");
  }
  else
  {
    PolygonMaxSeq seq{};
    size_t numberSeq = std::count_if(polygons.begin(), polygons.end(), std::bind(std::ref(seq), std::placeholders::_1, data));
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

bool hasIntersection(const smirnov::Polygon& p1, const smirnov::Polygon& p2)
{
  auto pair_p1 = std::minmax_element(p1.points.cbegin(), p1.points.cend());
  auto pair_p2 = std::minmax_element(p2.points.cbegin(), p2.points.cend());

  bool cond1 = (pair_p1.second->x < pair_p2.first->x) || (pair_p1.second->x == pair_p2.first->x && pair_p1.second->y < pair_p2.first->y);

  bool cond2 = (pair_p2.second->x < pair_p1.first->x) || (pair_p2.second->x == pair_p1.first->x && pair_p2.second->y < pair_p1.first->y);

  return !(cond1 || cond2);
}

void smirnov::doIntersections(const std::vector< Polygon >& data, std::istream& in, std::ostream& out)
{
  Polygon Polygon;
  in >> Polygon;
  if (!in)
  {
    throw std::logic_error("Wrong argument");
  }

  out << std::count_if(data.begin(), data.end(), std::bind(hasIntersection, std::placeholders::_1, Polygon)) << '\n';
}

template < class UnaryPredicate >
double getSumArea(const std::vector< smirnov::Polygon >& polygons, UnaryPredicate P)
{
  return std::accumulate(polygons.begin(), polygons.end(), 0.0, [&](double acc, const smirnov::Polygon& poly)
  {
    return P(poly) ? acc + smirnov::getArea(poly) : acc;
  });
}

double doAreaEven(const std::vector< smirnov::Polygon >& polygons)
{
  return getSumArea(polygons, smirnov::isEvenCountVertexes);
}

double doAreaOdd(const std::vector< smirnov::Polygon >& polygons)
{
  return getSumArea(polygons, smirnov::isOddCountVertexes);
}

struct SumAreas
{
  double operator()(double acc, const smirnov::Polygon& p) const
  {
    return acc + smirnov::getArea(p);
  }
};

double doAreaMean(const std::vector<smirnov::Polygon>& polygons)
{
  if (polygons.empty())
  {
    throw std::logic_error("NO POLYGONS FOR AREA MEAN COMMAND");
  }

  double result = std::accumulate(polygons.begin(), polygons.end(), 0.0, SumAreas{});
  result /= polygons.size();
  return result;
}

double doAreaNum(const std::vector< smirnov::Polygon >& polygons, size_t n)
{
  return getSumArea(polygons, std::bind(smirnov::isNCountVertexes, std::placeholders::_1, n));
}

