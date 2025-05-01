#include <iostream>
#include "geometry.hpp"


int main()
{
  demehin::Point pt;
  demehin::Polygon plg;
  std::cin >> pt;
  std::cout << pt.x << " " << pt.y << "\n";
  std::cin >> plg;
  for (size_t i = 0; i < plg.points.size(); i++)
  {
    std::cout << plg.points[i].x << plg.points[i].y << " ";
  }
}
