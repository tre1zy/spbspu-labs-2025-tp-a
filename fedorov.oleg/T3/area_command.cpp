#include <stdexcept>
#include <numeric>
#include <map>

#include "commands.hpp"
#include "functional.hpp"
#include "detail_commands.hpp"

namespace fedorov
{
  namespace
  {
    double areaOddHandler(const std::vector< Polygon > &polygons)
    {
      return std::accumulate(polygons.begin(), polygons.end(), 0.0, AreaOddAccumulator());
    }

    double areaEvenHandler(const std::vector< Polygon > &polygons)
    {
      return std::accumulate(polygons.begin(), polygons.end(), 0.0, AreaEvenAccumulator());
    }

    double areaMeanHandler(const std::vector< Polygon > &polygons)
    {
      if (polygons.empty())
      {
        throw std::logic_error("No polygons");
      }
      double total = std::accumulate(polygons.begin(), polygons.end(), 0.0, AreaMeanAccumulator());
      return total / polygons.size();
    }
  }

  void areaCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
  {
    std::string param;
    if (!(in >> param) || !isEOL(in))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    using Handler = double (*)(const std::vector< Polygon > &);
    std::map< std::string, Handler > handlers = {
        {"ODD", areaOddHandler}, {"EVEN", areaEvenHandler}, {"MEAN", areaMeanHandler}};

    try
    {
      auto it = handlers.find(param);
      if (it != handlers.end())
      {
        double result = it->second(polygons);
        outputFormattedArea(result, out);
      }
      else
      {
        size_t num = std::stoul(param);
        if (num < 3)
        {
          throw std::invalid_argument("Invalid vertex count");
        }
        double total = std::accumulate(polygons.begin(), polygons.end(), 0.0, AreaNumAccumulator(num));
        outputFormattedArea(total, out);
      }
    }
    catch (...)
    {
      out << "<INVALID COMMAND>\n";
    }
  }
}
