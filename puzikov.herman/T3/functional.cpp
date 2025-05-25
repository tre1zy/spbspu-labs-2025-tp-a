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
