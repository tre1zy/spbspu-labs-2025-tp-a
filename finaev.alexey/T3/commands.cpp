#include "commands.hpp"
#include <iomanip>
#include <streamGuard.hpp>
#include <algorithm>
#include <functional>
#include <iterator>

namespace
{
  bool isOdd(const finaev::Polygon& poly)
  {
    return poly.points.size() % 2 != 0;
  }

  bool isEven(const finaev::Polygon& poly)
  {
    return poly.points.size() % 2 == 0;
  }

  bool isNumOfVertex(const finaev::Polygon& poly, size_t s)
  {
    return poly.points.size() == s;
  }

  size_t countOdd(const std::vector< finaev::Polygon >& shapes)
  {
    return std::count_if(shapes.begin(), shapes.end(), isOdd);
  }

  size_t countEven(const std::vector< finaev::Polygon >& shapes)
  {
    return std::count_if(shapes.begin(), shapes.end(), isEven);
  }

  size_t countVertexes(const std::vector< finaev::Polygon >& shapes, size_t num)
  {
    auto compare = std::bind(isNumOfVertex, std::placeholders::_1, num);
    return std::count_if(shapes.begin(), shapes.end(), compare);
  }

  double areaOnePoly(const finaev::Polygon& poly)
  {
    if (poly.points.size() < 3)
    {
      return 0.0;
    }
    double res = 0.0;
    size_t s = poly.points.size();
    for (size_t i = 0; i < s; ++i)
    {
      int j = (i + 1) % s;
      res += (poly.points[i].x * poly.points[j].y) - (poly.points[i].y * poly.points[j].x);
    }
    return std::abs(res) / 2.0;
  }

  double areaOdd(const std::vector< finaev::Polygon >& shapes)
  {
    double res = 0.0;
    for (size_t i = 0; i < shapes.size(); ++i)
    {
      if (isOdd(shapes[i]))
      {
        res += areaOnePoly(shapes[i]);
      }
    }
    return res;
  }

  double areaEven(const std::vector< finaev::Polygon >& shapes)
  {
    double res = 0.0;
    for (size_t i = 0; i < shapes.size(); ++i)
    {
      if (isEven(shapes[i]))
      {
        res += areaOnePoly(shapes[i]);
      }
    }
    return res;
  }

  double areaMean(const std::vector< finaev::Polygon >& shapes)
  {
    if (shapes.empty())
    {
      throw std::out_of_range("<INVALID COMMAND>");
    }
    double res = 0.0;
    for (size_t i = 0; i < shapes.size(); ++i)
    {
      res += areaOnePoly(shapes[i]);
    }
    return res / shapes.size();
  }

  double areaVertexes(const std::vector< finaev::Polygon >& shapes, size_t num)
  {
    double res = 0.0;
    for (size_t i = 0; i < shapes.size(); ++i)
    {
      if (isNumOfVertex(shapes[i], num))
      {
        res += areaOnePoly(shapes[i]);
      }
    }
    return res;
  }

  bool compareVertex(const finaev::Polygon& poly1, const finaev::Polygon& poly2)
  {
    return poly1.points.size() < poly2.points.size();
  }

  bool compareArea(const finaev::Polygon& poly1, const finaev::Polygon& poly2)
  {
    return areaOnePoly(poly1) < areaOnePoly(poly2);
  }

  bool isDigit(const std::string& s)
  {
    return !s.empty() && std::all_of(s.begin(), s.end(), isdigit);
  }

  void maxArea(const std::vector< finaev::Polygon >& shapes, std::ostream& out)
  {
    auto max = (*std::max_element(shapes.begin(), shapes.end(), compareArea));
    finaev::StreamGuard guard(out);
    out << std::fixed << std::setprecision(1) << areaOnePoly(max);
  }

  void minArea(const std::vector< finaev::Polygon >& shapes, std::ostream& out)
  {
    auto min = (*std::max_element(shapes.begin(), shapes.end(), compareArea));
    finaev::StreamGuard guard(out);
    out << std::fixed << std::setprecision(1) << areaOnePoly(min);
  }

  void maxVertexes(const std::vector< finaev::Polygon >& shapes, std::ostream& out)
  {
    auto max = (*std::max_element(shapes.begin(), shapes.end(), compareVertex));
    out << max.points.size();
  }

  void minVertexes(const std::vector< finaev::Polygon >& shapes, std::ostream& out)
  {
    auto min = (*std::min_element(shapes.begin(), shapes.end(), compareVertex));
    out << min.points.size();
  }

  bool isSame(const finaev::Polygon& first, const finaev::Polygon& second)
  {
    if (first.points.size() != second.points.size())
    {
      return false;
    }
    int dx = second.points[0].x - first.points[0].x;
    int dy = second.points[0].y - first.points[0].y;
    for (size_t i = 1; i < first.points.size(); ++i)
    {
      if (second.points[i].x != first.points[i].x + dx || second.points[i].y != first.points[i].y + dy)
      {
        return false;
      }
    }
    return true;
  }

  bool isExactSame(const finaev::Polygon& first, const finaev::Polygon& second)
  {
    if (first.points.size() != second.points.size())
    {
      return false;
    }
    for (size_t i = 0; i < first.points.size(); ++i)
    {
      if (first.points[i].x != second.points[i].x || first.points[i].y != second.points[i].y)
      {
        return false;
      }
    }
    return true;
  }
}

void finaev::count(std::istream& in, std::ostream& out, const std::vector< Polygon >& shapes)
{
  std::string substr;
  in >> substr;
  std::map< std::string, std::function< size_t() > > cmd;
  cmd["ODD"] = std::bind(countOdd, std::cref(shapes));
  cmd["EVEN"] = std::bind(countEven, std::cref(shapes));
  try
  {
    if (isDigit(substr))
    {
      size_t s = std::stoul(substr);
      if (s < 3)
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }
      out << countVertexes(shapes, s);
    }
    else
    {
      out << cmd.at(substr)();
    }
  }
  catch (const std::out_of_range&)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
}

void finaev::area(std::istream& in, std::ostream& out, const std::vector< Polygon >& shapes)
{
  std::string substr;
  in >> substr;
  StreamGuard streamGuard(out);
  out << std::fixed << std::setprecision(1);
  std::map< std::string, std::function< double() > > cmd;
  cmd["ODD"] = std::bind(areaOdd, std::cref(shapes));
  cmd["EVEN"] = std::bind(areaEven, std::cref(shapes));
  cmd["MEAN"] = std::bind(areaMean, std::cref(shapes));
  try
  {
    if (isDigit(substr))
    {
      size_t s = std::stoul(substr);
      if (s < 3)
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }
      out << areaVertexes(shapes, s);
    }
    else
    {
      out << cmd.at(substr)();
    }
  }
  catch (const std::out_of_range&)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
}

void finaev::max(std::istream& in, std::ostream& out, const std::vector< Polygon >& shapes)
{
  if (shapes.empty())
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
  std::string substr;
  in >> substr;
  std::map< std::string, std::function< void() > > cmd;
  cmd["AREA"] = std::bind(maxArea, std::cref(shapes), std::ref(out));
  cmd["VERTEXES"] = std::bind(maxVertexes, std::cref(shapes), std::ref(out));
  try
  {
    cmd.at(substr)();
  }
  catch (const std::out_of_range&)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
}

void finaev::min(std::istream& in, std::ostream& out, const std::vector< Polygon >& shapes)
{
  if (shapes.empty())
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
  std::string substr;
  in >> substr;
  std::map< std::string, std::function< void() > > cmd;
  cmd["AREA"] = std::bind(minArea, std::cref(shapes), std::ref(out));
  cmd["VERTEXES"] = std::bind(minVertexes, std::cref(shapes), std::ref(out));
  try
  {
    cmd.at(substr)();
  }
  catch (const std::out_of_range&)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
}

void finaev::echo(std::istream& in, std::ostream& out, std::vector< Polygon >& shapes)
{
  Polygon poly;
  std::string temp;
  in >> poly;
  if (in.fail() || !in)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
  if (in.peek() != '\n')
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
  size_t countAdd = 0;
  std::vector< Polygon > res;
  for (auto it = shapes.begin(); it != shapes.end(); ++it)
  {
    res.push_back(*it);
    if (isExactSame(*it, poly))
    {
      res.push_back(*it);
      ++countAdd;
    }
  }
  shapes = std::move(res);
  out << countAdd;
}

void finaev::same(std::istream& in, std::ostream& out, const std::vector< Polygon >& shapes)
{
  Polygon poly;
  std::string temp;
  in >> poly;
  if (in.fail() || !in)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
  if (in.peek() != '\n')
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
  auto same = std::bind(isSame, poly, std::placeholders::_1);
  out << std::count_if(shapes.begin(), shapes.end(), same);
}

using CmdMap = std::map< std::string, std::function< void() > >;
CmdMap finaev::createCommandsHandler(std::istream& in, std::ostream& out, std::vector< Polygon >& shapes)
{
  CmdMap commands;
  commands["AREA"] = std::bind(finaev::area, std::ref(in), std::ref(out), std::cref(shapes));
  commands["COUNT"] = std::bind(finaev::count, std::ref(in), std::ref(out), std::cref(shapes));
  commands["MAX"] = std::bind(finaev::max, std::ref(in), std::ref(out), std::cref(shapes));
  commands["MIN"] = std::bind(finaev::min, std::ref(in), std::ref(out), std::cref(shapes));
  commands["SAME"] = std::bind(finaev::same, std::ref(in), std::ref(out), std::cref(shapes));
  commands["ECHO"] = std::bind(finaev::echo, std::ref(in), std::ref(out), std::ref(shapes));
  return commands;
}
