#include "permutations.hpp"

pilugina::PermutationPredicate::PermutationPredicate(const Polygon &ref):
  poly(ref)
{}

bool pilugina::PermutationPredicate::operator()(const Polygon &other) const
{
  return std::is_permutation(other.points.begin(), other.points.end(), poly.points.begin());
}
