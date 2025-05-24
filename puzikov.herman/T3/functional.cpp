#include "functional.hpp"
#include <algorithm>
#include <iterator>
#include <limits>
#include <numeric>

puzikov::AreaIt puzikov::maxAreaElement(AreaIt first, AreaIt last, const AreaComparator &comp)
{
  return std::max_element(first, last, comp);
}

puzikov::AreaIt puzikov::minAreaElement(AreaIt first, AreaIt last, const AreaComparator &comp)
{
  return std::min_element(first, last, comp);
}

puzikov::VertIt puzikov::maxVertElement(VertIt first, VertIt last, const VerticesComparator &comp)
{
  return std::max_element(first, last, comp);
}

puzikov::VertIt puzikov::minVertElement(VertIt first, VertIt last, const VerticesComparator &comp)
{
  return std::min_element(first, last, comp);
}

void puzikov::readPolygons(std::istream &in, std::vector< Polygon > &polygons)
{
  using input_it_t = std::istream_iterator< Polygon >;
  while (!in.eof())
  {
    std::copy(input_it_t(in), input_it_t(), std::back_inserter(polygons));
    if (in.fail())
    {
      in.clear();
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
