#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include "polygon.hpp"

namespace amine {

struct LineToPolygonConverter {
    Polygon operator()(const Line& line) const {
        Polygon poly;
        if (!line.content.empty() && parse_polygon(line.content, poly) && poly.points.size() >= 3) {
            return poly;
        }
        return Polygon{};
    }
};

struct EmptyPolygonChecker {
    bool operator()(const Polygon& p) const {
        return p.points.empty();
    }
};

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Error: missing filename argument\n";
        return 1;
    }

    std::ifstream infile(argv[1]);
    if (!infile)
    {
        std::cerr << "Error: could not open file\n";
        return 1;
    }

    std::vector<amine::Polygon> polygons;
    std::vector<amine::Line> lines;

    std::copy(std::istream_iterator<amine::Line>(infile),
              std::istream_iterator<amine::Line>(),
              std::back_inserter(lines));

    std::transform(lines.begin(), lines.end(),
                  std::back_inserter(polygons),
                  amine::LineToPolygonConverter{});

    polygons.erase(
    std::remove_if(polygons.begin(), polygons.end(),
                  amine::InvalidPolygonChecker{}),
    polygons.end()
    );

    amine::process_commands(polygons);
    return 0;
}
}
