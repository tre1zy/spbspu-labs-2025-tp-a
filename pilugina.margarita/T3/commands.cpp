#include "commands.hpp"
#include <format_guard.hpp>
#include <iomanip>
#include "areaCommands.hpp"
#include "countCommands.hpp"
#include "maxCommands.hpp"
#include "minCommands.hpp"

void pilugina::areaCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polys)
{
  FormatGuard g(out);
  out << std::setprecision(1) << calcAreas(polys, in);
}

void pilugina::maxCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polys)
{
  printMax(polys, in, out);
}

void pilugina::minCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polys)
{
  printMin(polys, in, out);
}

void pilugina::countCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polys)
{
  out << getCount(polys, in);
}

// void pilugina::sameCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polys)
// {

// }
