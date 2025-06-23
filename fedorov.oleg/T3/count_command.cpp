#include <stdexcept>
#include <algorithm>
#include <map>

#include "commands.hpp"
#include "functional.hpp"
#include "detail_commands.hpp"

namespace fedorov
{

  size_t countOddHandler(const std::vector< Polygon > &polygons)
  {
    return std::count_if(polygons.begin(), polygons.end(), CountOddPredicate());
  }

  size_t countEvenHandler(const std::vector< Polygon > &polygons)
  {
    return std::count_if(polygons.begin(), polygons.end(), CountEvenPredicate());
  }

  void countCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
  {
    std::string param;
    if (!(in >> param) || !isEOL(in))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    using Handler = size_t (*)(const std::vector< Polygon > &);
    std::map< std::string, Handler > handlers = {{"ODD", countOddHandler}, {"EVEN", countEvenHandler}};

    try
    {
      auto it = handlers.find(param);
      if (it != handlers.end())
      {
        size_t count = it->second(polygons);
        out << count << '\n';
      }
      else
      {
        size_t num = std::stoul(param);
        if (num < 3)
        {
          throw std::invalid_argument("Invalid vertex count");
        }
        size_t count = std::count_if(polygons.begin(), polygons.end(), CountNumPredicate(num));
        out << count << '\n';
      }
    }
    catch (...)
    {
      out << "<INVALID COMMAND>\n";
    }
  }
}
