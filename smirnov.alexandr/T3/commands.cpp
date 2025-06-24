#include "commands.hpp"
#include <map>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <limits>
#include <functional>
#include <guard.hpp>

void smirnov::areaCommand(const std::vector< Polygon > & polygons, std::istream & in, std::ostream & out)
{
  std::string arg;
  in >> arg;
  StreamGuard guard(out);
  out << std::fixed << std::setprecision(1);
  if (arg == "EVEN")
  {
    double sum = 0.0;
    for (const Polygon & poly : polygons)
    {
      if (poly.points.size() % 2 == 0)
      {
        sum += getArea(poly);
      }
    }
    out << sum << "\n";
    return;
  }
  if (arg == "ODD")
  {
    double sum = 0.0;
    for (const Polygon & poly : polygons)
    {
      if (poly.points.size() % 2 == 1)
      {
        sum += getArea(poly);
      }
    }
    out << sum << "\n";
    return;
  }
  if (arg == "MEAN")
  {
    if (polygons.empty())
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    double sum = 0.0;
    for (const Polygon & poly : polygons)
    {
      sum += getArea(poly);
    }
    out << (sum / polygons.size()) << "\n";
    return;
  }
  try
  {
    size_t n = std::stoul(arg);
    if (n < 3)
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    double sum = 0.0;
    for (const Polygon & poly : polygons)
    {
      if (poly.points.size() == n)
      {
        sum += getArea(poly);
      }
    }
    out << sum << "\n";
    return;
  }
  catch (...)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
}

void smirnov::maxCommand(const std::vector< Polygon > & polygons, std::istream & in, std::ostream & out)
{
  std::string arg;
  in >> arg;
  StreamGuard guard(out);
  out << std::fixed << std::setprecision(1);
  if (polygons.empty())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  if (arg == "AREA")
  {
    double max_area = getArea(polygons.front());
    for (const Polygon & poly : polygons)
    {
      double area = getArea(poly);
      if (area > max_area)
      {
        max_area = area;
      }
    }
    out << max_area << "\n";
    return;
  }
  if (arg == "VERTEXES")
  {
    size_t max_v = polygons.front().points.size();
    for (const Polygon & poly : polygons)
    {
      if (poly.points.size() > max_v)
      {
        max_v = poly.points.size();
      }
    }
    out << max_v << "\n";
    return;
  }
  throw std::logic_error("<INVALID COMMAND>");
}

void smirnov::minCommand(const std::vector< Polygon > & polygons, std::istream & in, std::ostream & out)
{
  std::string arg;
  in >> arg;
  StreamGuard guard(out);
  out << std::fixed << std::setprecision(1);
  if (polygons.empty())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  if (arg == "AREA")
  {
    double min_area = getArea(polygons.front());
    for (const Polygon & poly : polygons)
    {
      double area = getArea(poly);
      if (area < min_area)
      {
        min_area = area;
      }
    }
    out << min_area << "\n";
    return;
  }
  if (arg == "VERTEXES")
  {
    size_t min_v = polygons.front().points.size();
    for (const Polygon & poly : polygons)
    {
      if (poly.points.size() < min_v)
      {
        min_v = poly.points.size();
      }
    }
    out << min_v << "\n";
    return;
  }
  throw std::logic_error("<INVALID COMMAND>");
}

void smirnov::countCommand(const std::vector< Polygon > & polygons, std::istream & in, std::ostream & out)
{
  std::string arg;
  in >> arg;
  if (arg == "EVEN")
  {
    size_t cnt = 0;
    for (const Polygon & poly : polygons)
    {
      if (poly.points.size() % 2 == 0)
      {
        ++cnt;
      }
    }
    out << cnt << "\n";
    return;
  }
  if (arg == "ODD")
  {
    size_t cnt = 0;
    for (const Polygon & poly : polygons)
    {
      if (poly.points.size() % 2 == 1)
      {
        ++cnt;
      }
    }
    out << cnt << "\n";
    return;
  }
  try
  {
    size_t n = std::stoul(arg);
    if (n < 3)
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    size_t cnt = 0;
    for (const Polygon & poly : polygons)
    {
      if (poly.points.size() == n)
      {
        ++cnt;
      }
    }
    out << cnt << "\n";
    return;
  }
  catch (...)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
}

void smirnov::inframeCommand(const std::vector< Polygon > & polygons, std::istream & in, std::ostream & out)
{
  throw std::logic_error("<INVALID COMMAND>");
}

void smirnov::maxseqCommand(const std::vector< Polygon > & polygons, std::istream & in, std::ostream & out)
{
  throw std::logic_error("<INVALID COMMAND>");
}
