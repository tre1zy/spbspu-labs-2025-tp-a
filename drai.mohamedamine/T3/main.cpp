#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>

#include "polygon.hpp"
#include "commands.hpp"

namespace amine
{
  int main(int, char**)
  {
    std::istream_iterator<std::string> it(std::cin);
    std::istream_iterator<std::string> end;

    std::string buffer;
    std::vector<Polygon> polygons;

    std::for_each(it, end, [&](const std::string& token) {
      buffer += token + " ";
      if (std::count(buffer.begin(), buffer.end(), ' ') % 2 == 0)
      {
        Polygon poly;
        if (parse_polygon(buffer, poly))
        {
          polygons.push_back(std::move(poly));
        }
        buffer.clear();
      }
    });

    process_commands(polygons);
    return 0;
  }
}
