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
    using MaxSubcommands = std::map< std::string, std::function< double(const Polygon&) > >;
    using MinSubcommands = std::map< std::string, std::function< double(const Polygon&) > >;
    using CountSubcommands = std::map< std::string, std::function< bool(const Polygon&) > >;

    void area(const std::vector< Polygon >& polygons, std::istream& is, std::ostream& os);
    void max(const std::vector< Polygon >& polygons, std::istream& is, std::ostream& os);
    void min(const std::vector< Polygon >& polygons, std::istream& is, std::ostream& os);
    void count(const std::vector< Polygon >& polygons, std::istream& is, std::ostream& os);
    void perms(const std::vector< Polygon >& polygons, std::istream& is, std::ostream& os);
    void rects(const std::vector< Polygon >& polygons, std::istream&, std::ostream& os);
}
#endif
