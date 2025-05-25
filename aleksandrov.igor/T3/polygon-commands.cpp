#include "polygon-commands.hpp"
#include <algorithm>
#include <iterator>
#include <iostream>
#include <limits>
#include "io-utils.hpp"

namespace aleksandrov
{
  void getPolygons(std::istream& in, std::vector< Polygon >& polygons)
  {
    while (!in.eof())
    {
      std::copy(std::istream_iterator< Polygon >(in), std::istream_iterator< Polygon >(), std::back_inserter(polygons));
      if (!in)
      {
        in.clear();
        in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      }
    }
  }

  void area(const std::vector< Polygon >& polygons, std::ostream& out)
  {
    if (polygons.empty())
    {
      throw std::logic_error("There are no polygons!");
    }
    out << "area";
  }
  void max(const std::vector< Polygon >& polygons, std::ostream& out)
  {
    if (polygons.empty())
    {
      throw std::logic_error("There are no polygons!");
    }
    out << "max";
  }
  void min(const std::vector< Polygon >& polygons, std::ostream& out)
  {
    if (polygons.empty())
    {
      throw std::logic_error("There are no polygons!");
    }
    out << "min";
  }

  void count(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
  {
    if (polygons.empty())
    {
      throw std::logic_error("There are no polygons!");
    }
    std::string subcommand;
    in >> subcommand;
    if (subcommand == "EVEN")
    {
      struct EvenCounter
      {
        bool operator()(const Polygon& polygon) const
        {
          return polygon.points.size() % 2 == 0;
        }
      };
      out << std::count_if(polygons.begin(), polygons.end(), EvenCounter{});
    }
    else if (subcommand == "ODD")
    {
      struct OddCounter
      {
        bool operator()(const Polygon& polygon) const
        {
          return polygon.points.size() % 2 != 0;
        }
      };
      out << std::count_if(polygons.begin(), polygons.end(), OddCounter{});
    }
    else
    {
      throw std::logic_error("Unknown subcommand!");
    }
  }
}

