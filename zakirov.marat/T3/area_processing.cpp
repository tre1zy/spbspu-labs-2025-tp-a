#include "area_processing.hpp"
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <functional>
#include <limits>
#include "zakirov.marat/T2/stream_guardian.hpp"
#include "utilities.hpp"

void zakirov::process_area(const std::list< Polygon > & points, std::istream & in, std::ostream & out)
{
  std::string subcommand;
  in >> subcommand;
  if (subcommand == "EVEN")
  {
    out << count_sum_area(points, even_polygon_pred);
  }
  else if (subcommand == "ODD")
  {
    out << count_sum_area(points, odd_polygon_pred);
  }
  else if (subcommand == "MEAN")
  {
    std::vector< double > areas;
    std::transform(points.begin(), points.end(), std::back_inserter(areas), count_area);
    size_t size = areas.size();
    Guardian guard(out);
    out << std::fixed << std::setprecision(1) << std::accumulate(areas.begin(), areas.end(), 0.0) / size;
  }
  else if (std::all_of(subcommand.begin(), subcommand.end(), ::isdigit))
  {
    int a = std::stoi(subcommand);
    std::vector< Polygon > polygons;
    std::copy_if(points.begin(), points.end(),  std::back_inserter(polygons), std::bind(equal_vertexes_pred, get_vertex, a));
  }
}

void zakirov::process_less_area(const std::list< Polygon > & points, std::istream & in, std::ostream & out)
{
  std::string subcommand;
  in >> subcommand;
  zakirov::Polygon plgn;
  if (std::all_of(subcommand.begin(), subcommand.end(), ::isdigit))
  {
    zakirov::Point pnt;
    while (in >> pnt)
    {
      plgn.points_.push_back(pnt);
    }
  }

  if (!in)
  {
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    in.clear();
    return;
  }

  double base_area = count_area(plgn);
  std::vector< double > areas;
  std::transform(points.begin(), points.end(), std::back_inserter(areas), count_area);
  std::vector< double > less_areas;
  std::copy_if(areas.begin(), areas.end(),  std::back_inserter(less_areas), std::bind(less_area_pred, base_area, std::placeholders::_1));
  Guardian guard(out);
  out << std::fixed << std::setprecision(1) << std::accumulate(less_areas.begin(), less_areas.end(), 0.0);
}
