#include <functional>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <fstream>
#include <limits>
#include <map>
#include "geometry.hpp"

namespace aleksandrov
{
  struct DelimiterIO
  {
    char exp;
  };
  std::istream& operator>>(std::istream& in, DelimiterIO&& rhs)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    char c = '\0';
    if (!(in >> c) || c != rhs.exp)
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }
  std::istream& operator>>(std::istream& in, Point& rhs)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    using sep = DelimiterIO;
    return in >> sep{ '(' } >> rhs.x >> sep{ ';' } >> rhs.y >> sep{ ')' };
  }
  std::istream& operator>>(std::istream& in, Polygon& rhs)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    size_t vertexesCount = 0;
    if (!(in >> vertexesCount) || vertexesCount < 3)
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    std::vector< Point > points(vertexesCount);
    std::copy_n(std::istream_iterator< Point >(in), vertexesCount, points.begin());
    if (!in)
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    rhs.points = points;
    return in;
  }
  std::ostream& operator<<(std::ostream& out, const Point& rhs)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    return out << '(' << rhs.x << ';' << rhs.y << ')';
  }
  std::ostream& operator<<(std::ostream& out, const Polygon& rhs)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    out << rhs.points.size();
    for (size_t i = 0; i < rhs.points.size(); ++i)
    {
      out << ' ' << rhs.points[i];
    }
    return out;
  }

  void getShapes(std::istream& in, std::vector< Polygon >& polygons)
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

int main(int argc, char* argv[])
{
  using namespace aleksandrov;

  if (argc != 2)
  {
    std::cerr << "ERROR: Incorrect arguments!\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "ERROR: Incorrect file!\n";
    return 1;
  }

  std::vector< Polygon > polygons;
  getShapes(file, polygons);

  std::map< std::string, std::function< void() > > commands;
  commands["AREA"] = std::bind(area, std::cref(polygons), std::ref(std::cout));
  commands["MAX"] = std::bind(max, std::cref(polygons), std::ref(std::cout));
  commands["MIN"] = std::bind(min, std::cref(polygons), std::ref(std::cout));
  commands["COUNT"] = std::bind(count, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));

  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      commands.at(command)();
      std::cout << '\n';
    }
    catch (...)
    {
      if (std::cin.fail())
      {
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}

