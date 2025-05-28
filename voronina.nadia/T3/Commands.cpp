#include "Commands.h"

#include <exception>
#include <algorithm>
#include <iomanip>
#include <string>

#include "Delimiter.h"
#include "Iofmtguard.h"
#include "Subcommands.h"

const int MIN_AMOUNT_OF_VERTEXES = 3;

namespace voronina
{
  void area(const std::vector<Polygon>& shapes, std::istream& in, std::ostream& out)
  {
    if (in.peek() == '\n')
    {
      throw std::invalid_argument("ERROR: No parametrs");
    }

    iofmtguard ifmtguard(in);
    in >> std::noskipws;

    in >> DelimiterIO{ ' ' };

    if (in.peek() == '\n')
    {
      throw std::invalid_argument("ERROR: No parametrs");
    }

    std::string parametr = "";
    in >> parametr;

    if (in.peek() != '\n')
    {
      throw std::invalid_argument("ERROR: Wrong command");
    }
    iofmtguard ofmtguard(out);
    out << std::fixed << std::setprecision(1);
    if (parametr == "EVEN")
    {
      out << std::accumulate(shapes.cbegin(), shapes.cend(), 0.0, getAreaForEvenPolygons);
    }
    else if (parametr == "ODD")
    {
      out << std::accumulate(shapes.cbegin(), shapes.cend(), 0.0, getAreaForOddPolygons);
    }
    else if (parametr == "MEAN")
    {
      if (shapes.size() == 0)
      {
        throw std::invalid_argument("ERROR: No shapes");
      }
      out << std::accumulate
      (
        shapes.cbegin(), shapes.cend(), 0.0, getSumArea
      ) / shapes.size();
    }
    else
    {
      if (!std::all_of(parametr.cbegin(), parametr.cend(), isDigitBool))
      {
        throw std::invalid_argument("ERROR: Wrong argument");
      }

      int vertexes = std::stoi(parametr);

      if (vertexes >= MIN_AMOUNT_OF_VERTEXES)
      {
        out << std::accumulate
        (
          shapes.cbegin(), shapes.cend(), 0.0,
          std::bind
          (
            getAreaOfGivenAmountOfVertexes, std::placeholders::_1, std::placeholders::_2, vertexes
          )
        );
      }
      else
      {
        throw std::invalid_argument("ERROR: Invalid amount of vertexes");
      }
    }
  }
  void max(const std::vector<Polygon>& shapes, std::istream& in, std::ostream& out)
  {
    if (in.peek() == '\n')
    {
      throw std::invalid_argument("ERROR: No parametrs");
    }

    iofmtguard ifmtguard(in);
    in >> std::noskipws;

    in >> DelimiterIO{ ' ' };

    if (in.peek() == '\n')
    {
      throw std::invalid_argument("ERROR: No parametrs");
    }

    std::string parametr = "";
    in >> parametr;

    if (in.peek() != '\n')
    {
      throw std::invalid_argument("ERROR: Wrong command");
    }

    if (shapes.size() == 0)
    {
      throw std::invalid_argument("ERROR: No shapes");
    }

    if (parametr == "AREA")
    {
      iofmtguard ofmtguard(out);
      out << std::fixed << std::setprecision(1);
      out << getAreaOfPolygon(*std::max_element
      (
        shapes.cbegin(), shapes.cend(), areaComporator
      ));
    }
    else if (parametr == "VERTEXES")
    {
      out << (*std::max_element
      (
        shapes.cbegin(), shapes.cend(), vertexesComporator
      )).points.size();
    }
  }
  void min(const std::vector<Polygon>& shapes, std::istream& in, std::ostream& out)
  {
    if (in.peek() == '\n')
    {
      throw std::invalid_argument("ERROR: No parametrs");
    }

    iofmtguard ifmtguard(in);
    in >> std::noskipws;

    in >> DelimiterIO{ ' ' };

    if (in.peek() == '\n')
    {
      throw std::invalid_argument("ERROR: No parametrs");
    }

    std::string parametr = "";
    in >> parametr;

    if (in.peek() != '\n')
    {
      throw std::invalid_argument("ERROR: Wrong command");
    }

    if (shapes.size() == 0)
    {
      throw std::invalid_argument("ERROR: No shapes");
    }

    if (parametr == "AREA")
    {
      iofmtguard ofmtguard(out);
      out << std::fixed << std::setprecision(1);
      out << getAreaOfPolygon(*std::min_element
      (
        shapes.cbegin(), shapes.cend(), areaComporator
      ));
    }
    else if (parametr == "VERTEXES")
    {
      out << (*std::min_element
      (
        shapes.cbegin(), shapes.cend(), vertexesComporator
      )).points.size();
    }
  }
  void count(const std::vector<Polygon>& shapes, std::istream& in, std::ostream& out)
  {
    if (in.peek() == '\n')
    {
      throw std::invalid_argument("ERROR: No parametrs");
    }

    iofmtguard ifmtguard(in);
    in >> std::noskipws;

    in >> DelimiterIO{ ' ' };

    if (in.peek() == '\n')
    {
      throw std::invalid_argument("ERROR: No parametrs");
    }

    std::string parametr = "";
    in >> parametr;

    if (in.peek() != '\n')
    {
      throw std::invalid_argument("ERROR: Wrong command");
    }
    iofmtguard ofmtguard(out);
    out << std::fixed << std::setprecision(1);
    if (parametr == "EVEN")
    {
      out << std::count_if(shapes.cbegin(), shapes.cend(), isEven);
    }
    else if (parametr == "ODD")
    {
      out << std::count_if(shapes.cbegin(), shapes.cend(), isOdd);
    }
    else
    {
      if (!std::all_of(parametr.cbegin(), parametr.cend(), isDigitBool))
      {
        throw std::invalid_argument("ERROR: Wrong argument");
      }

      int vertexes = std::stoi(parametr);

      if (vertexes >= MIN_AMOUNT_OF_VERTEXES)
      {
        out << std::count_if(shapes.cbegin(), shapes.cend(),
          std::bind(hasGivenAmountOfVertexes, std::placeholders::_1, vertexes));
      }
      else
      {
        throw std::invalid_argument("ERROR: Invalid amount of vertexes");
      }
    }
  }
  void maxseq(const std::vector<Polygon>& shapes, std::istream& in, std::ostream& out)
  {
    if (in.peek() == '\n')
    {
      throw std::invalid_argument("ERROR: No parametrs");
    }

    iofmtguard ifmtguard(in);
    in >> std::noskipws;

    in >> DelimiterIO{ ' ' };

    Polygon polygon;
    in >> polygon;

    std::vector< int > supVector(shapes.size());
    std::transform
    (
      shapes.cbegin(), shapes.cend(), supVector.begin(),
      std::bind(isEqual, std::placeholders::_1, std::cref(polygon))
    );

    std::transform
    (
      supVector.begin() + 1, supVector.end(),
      supVector.begin(), supVector.begin() + 1,
      getMaxSeq
    );
    out << *std::max_element(supVector.cbegin(), supVector.cend());
  }
  void rightshapes(const std::vector<Polygon>& shapes, std::istream& in, std::ostream& out)
  {
    if (in.peek() != '\n')
    {
      throw std::invalid_argument("ERROR: Do not need any parametrs");
    }
    out << std::count_if
    (
      shapes.cbegin(), shapes.cend(), isThereRightAngleInPolygon
    );
  }
}

