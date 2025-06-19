#include "commands.hpp"
#include "polygon.hpp"

#include <vector>

int main()
{
  std::vector<amine::Polygon> polygons;
  amine::process_commands(polygons);
  return 0;
}
