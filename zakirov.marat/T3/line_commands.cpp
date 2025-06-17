#include "line_commands.hpp"
#include <algorithm>
#include <numeric>
#include <limits>
#include <functional>
#include <iomanip>
#include "stream_guardian.hpp"

namespace
{
  bool odd_polygon_pred(const zakirov::Polygon & polygon)
  {
    return polygon.points_.size() % 2 == 0;
  }

  bool even_polygon_pred(const zakirov::Polygon & polygon)
  {
    return polygon.points_.size() % 2 == 1;
  }

  bool less_area_pred(double area_base, double area_other)
  {
    return area_other < area_base;
  }

  size_t get_vertex(const zakirov::Polygon & polygon)
  {
    return polygon.points_.size();
  }

  double count_area(const zakirov::Polygon & plgn)
  {
    double result = 0;
    for (size_t i = 1; i < plgn.points_.size(); ++i)
    {
      result += plgn.points_[i-1].x_ * plgn.points_[i].y_ - plgn.points_[i - 1].y_ * plgn.points_[i].x_;
    }

    result += plgn.points_[plgn.points_.size() - 1].x_ * plgn.points_[0].y_ - plgn.points_[plgn.points_.size() - 1].y_ * plgn.points_[0].x_;
    result *= 0.5;
    return std::abs(result);
  }

  template <class P>
  double count_sum_area(const std::list< zakirov::Polygon > & points, P pred)
  {
    std::vector< Polygon > polygons;
    std::copy_if(points.begin(), points.end(),  std::back_inserter(polygons), even_polygon_pred);
    std::vector< double > areas;
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), count_area);
    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }

  int extract_num_until(const std::string & str, size_t start_pos, char el)
  {
    auto it = std::find(str.begin() + start_pos, str.end(), el);
    std::string substr(str.begin() + start_pos, it);
    if (!std::all_of(substr.begin(), substr.end(), ::isdigit))
    {
      throw std::logic_error("<NOT A COORDINATE>");
    }

    return std::stoi(substr);
  }

  zakirov::Point parce_point (const std::string & str)
  {
    zakirov::Point result{0, 0};
    size_t pos = 0;
    if (str[pos] != '(')
    {
      throw std::logic_error("<NOT A POINT>");
    }

    ++pos;
    result.x_ = extract_num_until(str, pos, ';');
    pos = str.find(";", pos) + 1;
    result.y_ = extract_num_until(str, pos, ')');
    pos = str.find(")", pos) + 1;

    if (pos != str.size())
    {
      throw std::logic_error("<NOT A POINT>");
    }

    return result;
  }
}

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
    out << std::accumulate(areas.begin(), areas.end(), 0.0) / size;
  }
  else if (std::all_of(subcommand.begin(), subcommand.end(), ::isdigit))
  {
    
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
    throw std::logic_error("<INVALID COMMAND>");
  }

  double base_area = count_area(plgn);
  std::vector< double > areas;
  std::transform(points.begin(), points.end(), std::back_inserter(areas), count_area);
  std::vector< double > less_areas;
  std::copy_if(areas.begin(), areas.end(),  std::back_inserter(less_areas), std::bind(less_area_pred, base_area, std::placeholders::_1));
  Guardian guard(out);
  out << std::fixed << std::setprecision(1) << std::accumulate(less_areas.begin(), less_areas.end(), 0.0);
}

void zakirov::find_extremum_max(const std::list< Polygon > & points, std::istream & in, std::ostream & out)
{
  std::string subcommand;
  in >> subcommand;
  if (subcommand == "AREA")
  {
    std::vector< double > areas;
    std::transform(points.begin(), points.end(), std::back_inserter(areas), count_area);
    Guardian guard(out);
    out << std::fixed << std::setprecision(1) << *std::max_element(areas.begin(), areas.end());
  }
  else if (subcommand == "VERTEXES")
  {
    std::vector< size_t > vertexes;
    std::transform(points.begin(), points.end(), std::back_inserter(vertexes), get_vertex);
    out << *std::max_element(vertexes.begin(), vertexes.end());
  }
}

void zakirov::find_extremum_min(const std::list< Polygon > & points, std::istream & in, std::ostream & out)
{
  std::string subcommand;
  in >> subcommand;
  if (subcommand == "AREA")
  {
    std::vector< double > areas;
    std::transform(points.begin(), points.end(), std::back_inserter(areas), count_area);
    Guardian guard(out);
    out << std::fixed << std::setprecision(1) << *std::min_element(areas.begin(), areas.end());
  }
  else if (subcommand == "VERTEXES")
  {
    std::vector< size_t > vertexes;
    std::transform(points.begin(), points.end(), std::back_inserter(vertexes), get_vertex);
    out << *std::min_element(vertexes.begin(), vertexes.end());
  }
}

void zakirov::count(const std::list< Polygon > & points, std::istream & in, std::ostream & out)
{

}

void zakirov::echo(std::list< Polygon > & points, std::istream & in, std::ostream & out)
{

}
