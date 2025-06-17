#include "line_commands.hpp"
#include <algorithm>
#include <numeric>
#include <limits>
#include <functional>
#include <iomanip>
#include "zakirov.marat/T2/stream_guardian.hpp"

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

  bool equal_vertexes_pred(double vertex_base, double vertex_other)
  {
    return vertex_base == vertex_other;
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

void zakirov::count(const std::list< Polygon > & points, std::istream & in, std::ostream & out)
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
  else if (std::all_of(subcommand.begin(), subcommand.end(), ::isdigit))
  {
    size_t i = std::stoi(subcommand);
    std::vector< double > vertexes;
    std::transform(points.begin(), points.end(), std::back_inserter(vertexes), get_vertex);
    size_t size = vertexes.size();
    out << std::accumulate(vertexes.begin(), vertexes.end(), 0.0);
  }
}

void zakirov::echo(std::list< Polygon > & points, std::istream & in, std::ostream & out)
{
  std::string subcommand;
  in >> subcommand;
  if (std::all_of(subcommand.begin(), subcommand.end(), ::isdigit))
  {
    Point pnt;
    Polygon plgn;
    while (in >> pnt)
    {
      plgn.points_.push_back(pnt);
    }

    for (auto it = points.begin(); it != points.end(); ++it)
    {
      if (std::equal((*it).points_.begin(), (*it).points_.end(), plgn.points_.begin()))
      {
        points.insert(it, plgn);
      }
    }
  }
}
