#ifndef TYPES_HPP
#define TYPES_HPP
#include <iostream>

namespace shapkov
{
  struct doubleScientificIO
  {
    double key;
  };
  struct ratioIO
  {
    std::pair< long long, unsigned long long > key;
  };
  struct stringIO
  {
    std::string& key;
  };
  std::istream& operator>>(std::istream& in, doubleScientificIO& rhs);
  std::istream& operator>>(std::istream& in, ratioIO& rhs);
  //std::istream& operator>>(std::istream& in, stringIO&& rhs);
}
#endif
