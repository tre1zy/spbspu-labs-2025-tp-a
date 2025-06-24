#ifndef OPENNINGINFO
#define OPENNINGINFO
#include <vector>
#include <string>

namespace finaev
{
  struct DebutMoves
  {
    std::vector< std::string > moves;
  };

  struct DebutName
  {
    std::vector< std::string > name;
  };

  struct DebutDescription
  {
    std::vector< std::string > description;
  };

  std::istream& operator>>(std::istream&, DebutMoves&);
  std::istream& operator>>(std::istream&, DebutName&);
  std::istream& operator>>(std::istream&, DebutDescription&);
  
  struct OpenningInfo
  {
    DebutMoves moves;
    DebutName name;
    DebutDescription description;
  };
}

#endif
