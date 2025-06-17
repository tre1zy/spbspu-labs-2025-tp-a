#include "vertex_processing.hpp"
#include <algorithm>
#include <numeric>
#include "utilities.hpp"

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
