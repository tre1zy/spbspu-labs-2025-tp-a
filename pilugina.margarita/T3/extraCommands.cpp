#include "extraCommands.hpp"
#include <algorithm>

int pilugina::EchoTransform::operator()(const Polygon &p) const
{
  if (p == poly)
  {
    result.push_back(p);
    result.push_back(p);
  }
  else
  {
    result.push_back(p);
  }
  return 0;
}

std::size_t pilugina::echo(std::vector< Polygon > &polys, const Polygon &poly)
{
  std::size_t count = std::count(polys.begin(), polys.end(), poly);

  std::vector< Polygon > result;
  result.reserve(polys.size() + count);
  std::for_each(polys.begin(), polys.end(), EchoTransform{poly, result});
  polys = std::move(result);
  return count;
}

pilugina::IntersectsWith::IntersectsWith(const Polygon &polygon):
  polygon_(polygon)
{
}

bool pilugina::IntersectsWith::operator()(const Polygon &other) const
{
  auto left = std::minmax_element(polygon_.points.begin(), polygon_.points.end());
  auto right = std::minmax_element(other.points.begin(), other.points.end());
  return !((*left.second < *right.first) || (*right.second < *left.first));
}

pilugina::PermutationPredicate::PermutationPredicate(const Polygon &ref):
  poly(ref)
{
}

bool pilugina::PermutationPredicate::operator()(const Polygon &other) const
{
  bool is_permutation = std::is_permutation(other.points.begin(), other.points.end(), poly.points.begin());
  return (poly.points.size() == other.points.size()) && is_permutation;
}
