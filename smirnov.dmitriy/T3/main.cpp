#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "polygon.hpp"
#include "commands.hpp"

int main()
{
  using namespace smirnov;

  std::vector<std::string> lines;
  std::string line;
  while (std::getline(std::cin, line))
  {
    lines.push_back(line);
  }

  std::vector<Polygon> polygons;
  size_t i = 0;
  for (; i < lines.size(); ++i)
  {
    std::string s = lines[i];
    auto first_non_ws = s.find_first_not_of(" \t\r\n");
    if (first_non_ws == std::string::npos)
    {
      ++i;
      break;
    }
    std::string token;
    std::istringstream iss0(s);
    if (!(iss0 >> token))
    {
      ++i;
      break;
    }
    if (token == "AREA" || token == "COUNT" || token == "MAX" || token == "MIN" ||
        token == "RECTS" || token == "MAXSEQ" || token == "INTERSECTIONS" || token == "ECHO" ||
        token == "INFRAME" || token == "PERMS" || token == "RMECHO" || token == "SAME")
    {
      break;
    }
    std::istringstream iss(s);
    Polygon p;
    if (iss >> p)
    {
      polygons.push_back(p);
    }
  }

  for (; i < lines.size(); ++i)
  {
    std::string cmdline = lines[i];
    std::istringstream iss(cmdline);
    std::string cmd;
    if (!(iss >> cmd))
    {
      continue;
    }
    try
    {
      if (cmd == "AREA")
      {
        doAreaCommand(polygons, iss, std::cout);
      }
      else if (cmd == "COUNT")
      {
        doCountCommand(polygons, iss, std::cout);
      }
      else if (cmd == "MAX")
      {
        doMaxCommand(polygons, iss, std::cout);
      }
      else if (cmd == "MIN")
      {
        doMinCommand(polygons, iss, std::cout);
      }
      else if (cmd == "RECTS")
      {
        doRectsCommand(polygons, std::cout);
      }
      else if (cmd == "MAXSEQ")
      {
        doMaxseqCommand(polygons, iss, std::cout);
      }
      else if (cmd == "INTERSECTIONS")
      {
        doIntersections(polygons, iss, std::cout);
      }
      else
      {
        throw std::logic_error("INVALID COMMAND");
      }
    }
    catch (const std::logic_error& e)
    {
      std::cout << "<" << e.what() << ">" << '\n';
    }
  }

  return 0;
}

