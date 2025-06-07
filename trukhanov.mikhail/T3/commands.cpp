#include "commands.hpp"
#include <algorithm>
#include <numeric>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
#include "functors.hpp"
#include "polygon.hpp"
#include "data_input.hpp"
#include "stream_guard.hpp"

namespace trukhanov
{
  void area(std::istream& in, std::ostream& out, const Polygon_vector& src)
  {
    StreamGuard guard(out);
    std::string subcommand;
    in >> subcommand;

    out << std::fixed << std::showpoint << std::setprecision(1);

    if (subcommand == "EVEN")
    {
      std::vector<Polygon> filtered;
      std::copy_if(src.begin(), src.end(), std::back_inserter(filtered), isEven);

      std::vector<double> areas(filtered.size());
      std::transform(filtered.begin(), filtered.end(), areas.begin(), getArea);

      double total = std::accumulate(areas.begin(), areas.end(), 0.0);
      out << total << '\n';
    }
    else if (subcommand == "ODD")
    {
      std::vector<Polygon> filtered;
      std::copy_if(src.begin(), src.end(), std::back_inserter(filtered), isOdd);

      std::vector<double> areas(filtered.size());
      std::transform(filtered.begin(), filtered.end(), areas.begin(), getArea);

      double total = std::accumulate(areas.begin(), areas.end(), 0.0);
      out << total << '\n';
    }
    else if (subcommand == "MEAN")
    {
      if (src.empty())
      {
        out << "<INVALID COMMAND>\n";
      }
      else
      {
        std::vector<double> areas(src.size());
        std::transform(src.begin(), src.end(), areas.begin(), getArea);

        double total = std::accumulate(areas.begin(), areas.end(), 0.0);
        out << (total / static_cast<double>(areas.size())) << '\n';
      }
    }
    else if (std::all_of(subcommand.begin(), subcommand.end(), ::isdigit))
    {
      size_t size = std::stoull(subcommand);
      if (size < 3)
      {
        out << "<INVALID COMMAND>\n";
      }
      else
      {
        std::vector<Polygon> filtered;
        std::copy_if(src.begin(), src.end(), std::back_inserter(filtered), isSize{ size });

        std::vector<double> areas(filtered.size());
        std::transform(filtered.begin(), filtered.end(), areas.begin(), getArea);

        double total = std::accumulate(areas.begin(), areas.end(), 0.0);
        out << total << '\n';
      }
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
        out << "<INVALID COMMAND>\n";
        return;
      }
      out << getArea(*std::max_element(src.begin(), src.end(), CompareByArea())) << '\n';
    }
    else if (subcommand == "VERTEXES")
    {
      if (src.empty())
      {
        out << "<INVALID COMMAND>\n";
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
        out << "<INVALID COMMAND>\n";
        return;
      }
      out << getArea(*std::min_element(src.begin(), src.end(), CompareByArea())) << '\n';
    }
    else if (subcommand == "VERTEXES")
    {
      if (src.empty())
      {
        out << "<INVALID COMMAND>\n";
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
    StreamGuard guard(out);
    std::string subcommand;
    in >> subcommand;

    std::vector< Polygon > filtered;
    std::copy_if(src.begin(), src.end(), std::back_inserter(filtered), PolygonHasMinSize{});

    if (subcommand == "EVEN")
    {
      out << std::count_if(filtered.begin(), filtered.end(), isEven) << '\n';
    }
    else if (subcommand == "ODD")
    {
      out << std::count_if(filtered.begin(), filtered.end(), isOdd) << '\n';
    }
    else if (std::all_of(subcommand.begin(), subcommand.end(), ::isdigit))
    {
      size_t size = std::stoull(subcommand);
      if (size < 3)
      {
        out << "<INVALID COMMAND>\n";
      }
      else
      {
        out << std::count_if(filtered.begin(), filtered.end(), isSize{ size }) << '\n';
      }
    }
    else
    {
      out << "<INVALID COMMAND>\n";
    }
  }

  void lessArea(std::istream& in, std::ostream& out, const Polygon_vector& src)
  {
    StreamGuard guard(out);
    Polygon polygon;
    in >> polygon;

    HasDuplicates checkDuplicates;
    if (!in || polygon.points.size() < 3 || checkDuplicates(polygon))
    {
      out << "<INVALID COMMAND>\n";
      in.clear();
      in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      return;
    }

    double refArea = getArea(polygon);
    size_t count = std::count_if(src.begin(), src.end(), LessArea{ refArea });
    out << count << '\n';
  }

  void right(std::istream&, std::ostream& out, const Polygon_vector& src)
  {
    StreamGuard guard(out);
    out << std::count_if(src.begin(), src.end(), isRight) << '\n';
  }
}
