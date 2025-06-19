#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include "commands.hpp"
#include "polygon.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
  using namespace amine;

  std::vector<Polygon> polygons;

  std::istream_iterator<std::string> it(std::cin);
  std::istream_iterator<std::string> end;

  std::vector<std::string> args(it, end);

  if (!args.empty())
  {
    execute_command(args.front(), std::vector<std::string>(args.begin() + 1, args.end()), polygons);
  }

  return 0;
}
