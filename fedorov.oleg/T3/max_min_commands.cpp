#include <functional>
#include <map>

#include "commands.hpp"
#include "functional.hpp"
#include "detail_commands.hpp"

namespace fedorov
{
  void maxCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polys)
  {
    if (polys.empty())
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    std::string param;
    if (!(in >> param) || !isEOL(in))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    using CommandFunc = std::function< void(std::ostream &, const std::vector< Polygon > &) >;
    std::map< std::string, CommandFunc > subcommands = {{"AREA", maxArea}, {"VERTEXES", maxVertices}};

    auto it = subcommands.find(param);
    if (it != subcommands.end())
    {
      it->second(out, polys);
    }
    else
    {
      out << "<INVALID COMMAND>\n";
    }
  }

  void minCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polys)
  {
    if (polys.empty())
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    std::string param;
    if (!(in >> param) || !isEOL(in))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    using CommandFunc = std::function< void(std::ostream &, const std::vector< Polygon > &) >;
    std::map< std::string, CommandFunc > subcommands = {{"AREA", minArea}, {"VERTEXES", minVertices}};

    auto it = subcommands.find(param);
    if (it != subcommands.end())
    {
      it->second(out, polys);
    }
    else
    {
      out << "<INVALID COMMAND>\n";
    }
  }
}
