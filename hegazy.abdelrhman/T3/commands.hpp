#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include <vector>
#include "geometry.hpp"

namespace bob
{
    class Iofmtguard
    {
    public:
        explicit Iofmtguard(std::ostream& os);
        ~Iofmtguard();
    private:
        std::ostream& os_;
        std::ios::fmtflags flags_;
        char fill_;
    };

    void printAreaSum(std::istream& input, const std::vector<geom::Polygon>& polygons, std::ostream& output);
    void printMaxValueOf(std::istream& input, const std::vector<geom::Polygon>& polygons, std::ostream& output);
    void printMinValueOf(std::istream& input, const std::vector<geom::Polygon>& polygons, std::ostream& output);
    void printCountOf(std::istream& input, const std::vector<geom::Polygon>& polygons, std::ostream& output);
    void printLessAreaCnt(std::istream& input, const std::vector<geom::Polygon>& polygons, std::ostream& output);
    void printIntersectionsCnt(std::istream& input, const std::vector<geom::Polygon>& polygons, std::ostream& output);
}

#endif