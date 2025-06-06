#include "commands.hpp"
#include <algorithm>
#include <iomanip>
#include <numeric>
#include "format_guard.hpp"

void puzikov::areaCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return;
  }
  FormatGuard guard(in);
  out << std::fixed << std::setprecision(1);

  std::string param;
  if (!(in >> param))
  {
    return;
  }

  double areaSum;
  try
  {
    areaSum = std::accumulate(polygons.begin(), polygons.end(), 0.0, AreaAccumulator(param));
  }
  catch (...)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  if (param == "MEAN")
  {
    out << areaSum / polygons.size() << '\n';
  }
  else
  {
    out << areaSum << '\n';
  }
}

void puzikov::minmaxCommand(
    std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons, AreaAlgo areaAlgo, VertAlgo vertAlgo)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return;
  }
  FormatGuard guard(in);
  out << std::fixed << std::setprecision(1);

  std::string param;
  if (!(in >> param))
  {
    return;
  }

  if (param == "AREA")
  {
    if (polygons.empty())
    {
      out << 0.0 << '\n';
      return;
    }
    auto it = areaAlgo(polygons.begin(), polygons.end(), AreaComparator {});
    out << calcPolygonArea(*it) << '\n';
  }
  else if (param == "VERTEXES")
  {
    if (polygons.empty())
    {
      out << 0 << '\n';
      return;
    }
    auto it = vertAlgo(polygons.begin(), polygons.end(), VerticesComparator {});
    out << it->points.size() << '\n';
  }
}

void puzikov::maxCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  minmaxCommand(in, out, polygons, maxAreaElement, maxVertElement);
}

void puzikov::minCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  minmaxCommand(in, out, polygons, minAreaElement, minVertElement);
}

void puzikov::countCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return;
  }

  std::string param;
  if (!(in >> param))
  {
    return;
  }

  unsigned counter;
  try
  {
    counter = std::accumulate(polygons.begin(), polygons.end(), 0, ShapesAccumulator(param));
  }
  catch (...)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  out << counter << '\n';
}

void puzikov::rmEchoCommand(std::istream &in, std::ostream &out, std::vector< Polygon > &polygons)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return;
  }

  std::string params;
  if (!(std::getline(in, params)))
  {
    return;
  }
  std::istringstream iss(params);
  Polygon reference;
  if (!(iss >> reference))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto last = std::unique(polygons.begin(), polygons.end(), RmEchoPredicate {reference});
  unsigned counter = std::distance(last, polygons.end());
  polygons.erase(last, polygons.end());

  out << counter << '\n';
}

void puzikov::sameCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return;
  }

  std::string params;
  if (!(std::getline(in, params)))
  {
    return;
  }
  std::istringstream iss(params);
  Polygon reference;
  if (!(iss >> reference))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  unsigned count = std::count_if(polygons.begin(), polygons.end(), IsTranslationCongruent(reference));
  out << count << '\n';
}
