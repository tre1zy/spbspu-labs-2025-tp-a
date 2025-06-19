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
    using VecPolygon = std::vector< Polygon >;
    using AreaSubs = std::map< std::string, std::function< double(double, const Polygon&) > >;
    using MaxSubs = std::map< std::string, std::function< double(const Polygon&) > >;
    using MinSubs = std::map< std::string, std::function< double(const Polygon&) > >;
    using CountSubs = std::map< std::string, std::function< bool(const Polygon&) > >;
    using CommandHandler = std::map< std::string, std::function< void() > >;

    CommandHandler createCommandHandler(const VecPolygon& polygons);

    void area(const VecPolygon& polygons, std::istream& is, std::ostream& os);
    void max(const VecPolygon& polygons, std::istream& is, std::ostream& os);
    void min(const VecPolygon& polygons, std::istream& is, std::ostream& os);
    void count(const VecPolygon& polygons, std::istream& is, std::ostream& os);
    void perms(const VecPolygon& polygons, std::istream& is, std::ostream& os);
    void rects(const VecPolygon& polygons, std::ostream& os);
}
#endif
