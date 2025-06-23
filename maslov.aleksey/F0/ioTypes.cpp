#include "ioTypes.hpp"

std::ostream & maslov::operator<<(std::ostream & out, const Words & wordPair)
{
  return out << wordPair.first << ' ' << wordPair.second;
}
