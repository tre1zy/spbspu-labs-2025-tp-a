#ifndef OPENNINGINFO
#define OPENNINGINFO
#include <vector>
#include <string>

namespace finaev
{
  struct OpenningInfo
  {
    std::vector< std::string > moves;
    std::string name;
    std::string description;
  };
}

#endif