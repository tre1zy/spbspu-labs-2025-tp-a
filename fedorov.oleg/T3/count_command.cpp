#include <stdexcept>
#include <algorithm>

#include "commands.hpp"
#include "functional.hpp"
#include "detail_commands.hpp"

namespace fedorov
{
  void countCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
  {
    std::string param;
    if (!(in >> param) || !isEOL(in))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    if (param == "ODD")
    {
      out << std::count_if(polygons.begin(), polygons.end(), CountOddPredicate()) << '\n';
    }
    else if (param == "EVEN")
    {
      out << std::count_if(polygons.begin(), polygons.end(), CountEvenPredicate()) << '\n';
    }
    else
    {
      try
      {
        size_t num = std::stoul(param);
        if (num < 3)
        {
          throw std::invalid_argument("Invalid vertex count");
        }
        out << std::count_if(polygons.begin(), polygons.end(), CountNumPredicate(num)) << '\n';
      }
      catch (...)
      {
        out << "<INVALID COMMAND>\n";
      }
    }
  }
}
