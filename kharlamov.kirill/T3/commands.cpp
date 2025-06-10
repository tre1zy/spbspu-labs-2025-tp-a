#include "commands.h"
#include <algorithm>
#include "polygon.h"
#include "scopeguard.h"

namespace kharlamov
{
  void maxSeq(std::istream& in, std::ostream& out, const PolygonVec& polygons)
  {
    Polygon target;
    in >> target;
    if (!in || in.peek() != '\n')
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    size_t maxCount = 0;
    size_t currentCount = 0;
    for (const auto& poly : polygons)
    {
      if (poly == target)
      {
        currentCount++;
        maxCount = std::max(maxCount, currentCount);
      }
      else
      {
        currentCount = 0;
      }
    }
      out << maxCount << "\n";
  }

  void same(std::istream& in, std::ostream& out, const PolygonVec& polygons)
  {
    Polygon target;
    in >> target;
    if (!in || in.peek() != '\n')
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    size_t count = 0;
    for (const auto& poly : polygons)
    {
      if (poly.points.size() == target.points.size())
      {
        bool isSame = true;
        for (size_t i = 0; i < poly.points.size(); ++i)
        {
          if (poly.points[i].x != target.points[i].x || poly.points[i].y != target.points[i].y)
          {
            isSame = false;
            break;
          }
        }
        if (isSame) count++;
      }
    }
    out << count << "\n";
  }
}
