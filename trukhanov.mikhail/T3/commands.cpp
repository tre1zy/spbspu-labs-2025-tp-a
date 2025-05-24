#include "commands.hpp"
#include <algorithm>
#include <numeric>
#include <string>
#include <iomanip>
#include "functors.hpp"
#include "polygon.hpp"
#include "data_input.hpp"
#include "stream_guard.hpp"

namespace trukhanov
{
  struct AccumulateEven
  {
    double operator()(double sum, const Polygon& p) const
    {
      return isEven(p) ? sum + getArea(p) : sum;
    }
  };

  struct AccumulateOdd
  {
    double operator()(double sum, const Polygon& p) const
    {
      return isOdd(p) ? sum + getArea(p) : sum;
    }
  };

  struct AccumulateBySize
  {
    size_t size;
    AccumulateBySize(size_t s) : size(s) {}
    double operator()(double sum, const Polygon& p) const
    {
      return (p.points.size() == size) ? sum + getArea(p) : sum;
    }
  };

  void area(std::istream& in, std::ostream& out, const Polygon_vector& src)
  {
    StreamGuard guard(out);
    std::string subcommand;
    in >> subcommand;

    out << std::fixed << std::showpoint << std::setprecision(1);

    if (subcommand == "EVEN")
    {
      double total = std::accumulate(src.begin(), src.end(), 0.0, AccumulateEven());
      out << total << '\n';
    }
    else if (subcommand == "ODD")
    {
      double total = std::accumulate(src.begin(), src.end(), 0.0, AccumulateOdd());
      out << total << '\n';
    }
    else if (subcommand == "MEAN")
    {
      if (src.empty())
      {
        out << "At least one polygon is required to compute the mean\n";
      }
      else
      {
        double total = std::accumulate(src.begin(), src.end(), 0.0, AccumulateAll());
        double mean = total / static_cast<double>(src.size());
        out << mean << '\n';
      }
    }
    else if (std::all_of(subcommand.begin(), subcommand.end(), ::isdigit))
    {
      size_t size = std::stoull(subcommand);
      double total = std::accumulate(src.begin(), src.end(), 0.0, AccumulateBySize(size));
      out << total << '\n';
    }
    else
    {
      out << "<INVALID COMMAND>\n";
    }
  }

  void max(std::istream& in, std::ostream& out, const Polygon_vector& src)
  {
    StreamGuard guard(out);
    std::string subcommand;
    in >> subcommand;

    out << std::fixed << std::showpoint << std::setprecision(1);

    if (subcommand.empty() || subcommand == "AREA")
    {
      if (src.empty())
      {
        out << "0\n";
        return;
      }
      out << getArea(*std::max_element(src.begin(), src.end(), CompareByArea())) << '\n';
    }
    else if (subcommand == "VERTEXES")
    {
      if (src.empty())
      {
        out << "0\n";
        return;
      }
      out << std::max_element(src.begin(), src.end(), CompareByVertexes())->points.size() << '\n';
    }
    else
    {
      out << "<INVALID COMMAND>\n";
    }
  }

  void min(std::istream& in, std::ostream& out, const Polygon_vector& src)
  {
    StreamGuard guard(out);
    std::string subcommand;
    in >> subcommand;

    out << std::fixed << std::showpoint << std::setprecision(1);

    if (subcommand.empty() || subcommand == "AREA")
    {
      if (src.empty())
      {
        out << "0\n";
        return;
      }
      out << getArea(*std::min_element(src.begin(), src.end(), CompareByArea())) << '\n';
    }
    else if (subcommand == "VERTEXES")
    {
      if (src.empty())
      {
        out << "0\n";
        return;
      }
      out << std::min_element(src.begin(), src.end(), CompareByVertexes())->points.size() << '\n';
    }
    else
    {
      out << "<INVALID COMMAND>\n";
    }
  }


  void count(std::istream& in, std::ostream& out, const Polygon_vector& src)
  {
    std::string subcommand;
    in >> subcommand;

    if (subcommand == "EVEN")
    {
      out << std::count_if(src.begin(), src.end(), isEven) << '\n';
    }
    else if (subcommand == "ODD")
    {
      out << std::count_if(src.begin(), src.end(), isOdd) << '\n';
    }
    else if (std::all_of(subcommand.begin(), subcommand.end(), ::isdigit))
    {
      size_t size = std::stoull(subcommand);
      out << std::count_if(src.begin(), src.end(), isSize{ size }) << '\n';
    }
    else
    {
      out << "<INVALID COMMAND>\n";
    }
  }

  void lessArea(std::istream& in, std::ostream& out, const Polygon_vector& src)
  {
    Polygon polygon;
    in >> polygon;
    if (!in)
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    double refArea = getArea(polygon);
    size_t count = std::count_if(src.begin(), src.end(), LessArea{ refArea });
    out << count << '\n';
  }

  void right(std::istream&, std::ostream& out, const Polygon_vector& src)
  {
    out << std::count_if(src.begin(), src.end(), isRight) << '\n';
  }
}
