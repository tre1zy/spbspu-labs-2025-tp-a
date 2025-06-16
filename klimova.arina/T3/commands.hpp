#ifndef COMMANDS_H
#define COMMANDS_H

#include "polygon.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <functional>

namespace klimova
{
    using namespace std::placeholders;
    using AreaSubcommands = std::map< std::string, std::function< double(double, const Polygon&) > >;
    void area(const std::vector< Polygon >& polygons, std::istream& is, std::ostream& os);
}
#endif
