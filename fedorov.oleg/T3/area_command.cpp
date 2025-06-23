#include <stdexcept>
#include <numeric>

#include "commands.hpp"
#include "functional.hpp"
#include "detail_commands.hpp"

namespace fedorov
{
  void areaCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
  {
    std::string param;
    if (!(in >> param) || !isEOL(in))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    try
    {
      if (param == "ODD")
      {
        double total = std::accumulate(polygons.begin(), polygons.end(), 0.0, AreaOddAccumulator());
        outputFormattedArea(total, out);
      }
      else if (param == "EVEN")
      {
        double total = std::accumulate(polygons.begin(), polygons.end(), 0.0, AreaEvenAccumulator());
        outputFormattedArea(total, out);
      }
      else if (param == "MEAN")
      {
        if (polygons.empty())
        {
          throw std::logic_error("No polygons");
        }
        double total = std::accumulate(polygons.begin(), polygons.end(), 0.0, AreaMeanAccumulator());
        outputFormattedArea(total / polygons.size(), out);
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
