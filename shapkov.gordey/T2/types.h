#ifndef TYPES_H
#define TYPES_H
#include <iostream>

namespace shapkov
{
  struct doubleScientificIO
  {
    double key;
  }
  struct ratioIO
  {
    std::pair< long long, unsigned long long > key;
  }
  struct stringIO
  {
    std::string key;
  }
  std::istream& operator<<(std::istream& in, const doubleScientificIO& rhs);
  std::istream& operator<<(std::istream& in, const ratioIO& rhs);
  std::istream& operator<<(std::istream& in, const stringIO& rhs);
}
#endif
