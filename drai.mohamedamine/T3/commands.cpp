#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <vector>
#include "polygon.hpp"

namespace amine {

std::istream& operator>>(std::istream& is, Line& line)
{
    std::getline(is, line.content);
    return is;
}

struct PolyEqual {
    const Polygon& ref;
    bool operator()(const Polygon& p) const {
        return p.points.size() == ref.points.size() &&
               std::equal(p.points.begin(), p.points.end(), ref.points.begin());
    }
};

struct ConsecutiveEqual {
    PolyEqual eq;
    bool operator()(const Polygon& a, const Polygon& b) const {
        return eq(a) && eq(b);
    }
};

void process_rmecho(std::vector<Polygon>& polygons, const Polygon& query)
{
    size_t initial_size = polygons.size();
    polygons.erase(
        std::unique(polygons.begin(), polygons.end(), ConsecutiveEqual{PolyEqual{query}}),
        polygons.end()
    );
    std::cout << (initial_size - polygons.size()) << "\n";
}

struct AreaFilter {
    enum Mode { EVEN, ODD, MEAN, COUNT };
    Mode mode;
    int vertex_count = 0;
    double operator()(double acc, const Polygon& p) const {
        if (p.points.size() < 3) return acc;
        const double area = compute_area(p);
        switch(mode) {
            case EVEN: return p.points.size() % 2 == 0 ? acc + area : acc;
            case ODD: return p.points.size() % 2 != 0 ? acc + area : acc;
            case MEAN: return acc + area;
            default: return static_cast<int>(p.points.size()) == vertex_count ?
                   acc + area : acc;
        }
    }
};

struct CountFilter {
    enum Mode { EVEN, ODD, COUNT };
    Mode mode;
    int vertex_count = 0;
    bool operator()(const Polygon& p) const {
        const size_t vertices = p.points.size();
        if (vertices < 3) return false;
        switch(mode) {
            case EVEN: return vertices % 2 == 0;
            case ODD: return vertices % 2 != 0;
            default: return static_cast<int>(vertices) == vertex_count;
        }
    }
};

struct AreaComparator {
    bool operator()(const Polygon& a, const Polygon& b) const {
        return compute_area(a) < compute_area(b);
    }
};

struct VertexCountComparator {
    bool operator()(const Polygon& a, const Polygon& b) const {
        return a.points.size() < b.points.size();
    }
};

void process_command(const std::vector<Polygon>& polygons, const std::string& cmd, std::istringstream& iss)
{
    bool invalid = false;
    bool printDouble = false;
    double dblResult = 0.0;
    int intResult = 0;

    else if (cmd == "AREA") {
    std::string arg;
    iss >> arg;
    AreaFilter filter;

    if (arg == "EVEN") filter.mode = AreaFilter::EVEN;
    else if (arg == "ODD") filter.mode = AreaFilter::ODD;
    else if (arg == "MEAN") {
        if (polygons.empty()) invalid = true;
        else {
            filter.mode = AreaFilter::MEAN;
            dblResult = std::accumulate(polygons.begin(), polygons.end(), 0.0, filter) /
                        std::count_if(polygons.begin(), polygons.end(), 
                                    [](const Polygon& p){ return p.points.size() >= 3; });
            printDouble = true;
        }
    }
        else if (std::all_of(arg.begin(), arg.end(), ::isdigit)) {
            int num = std::stoi(arg);
            if (num < 3) invalid = true;
            else {
                filter.mode = AreaFilter::COUNT;
                filter.vertex_count = num;
                dblResult = std::accumulate(polygons.begin(), polygons.end(), 0.0, filter);
                printDouble = true;
            }
        }
        else invalid = true;
    }
    else if (cmd == "MAX" || cmd == "MIN") {
        std::string arg;
        iss >> arg;
        if (polygons.empty()) invalid = true;
        else if (arg == "AREA") {
            auto it = cmd == "MAX"
                ? std::max_element(polygons.begin(), polygons.end(), AreaComparator{})
                : std::min_element(polygons.begin(), polygons.end(), AreaComparator{});
            dblResult = compute_area(*it);
            printDouble = true;
        }
        else if (arg == "VERTEXES") {
            auto it = cmd == "MAX"
                ? std::max_element(polygons.begin(), polygons.end(), VertexCountComparator{})
                : std::min_element(polygons.begin(), polygons.end(), VertexCountComparator{});
            intResult = it->points.size();
        }
        else invalid = true;
    }
    else if (cmd == "COUNT") {
    std::string arg;
    iss >> arg;
    CountFilter filter;

    if (arg == "EVEN") filter.mode = CountFilter::EVEN;
    else if (arg == "ODD") filter.mode = CountFilter::ODD;
    else if (std::all_of(arg.begin(), arg.end(), ::isdigit)) {
        int num = std::stoi(arg);
        if (num < 3) invalid = true;
        else {
            filter.mode = CountFilter::COUNT;
            filter.vertex_count = num;
        }
    }
    else invalid = true;

    if (!invalid) {
        intResult = std::count_if(polygons.begin(), polygons.end(), filter);
     }
    }
    else if (cmd == "INTERSECTIONS") {
        std::string rest;
        std::getline(iss, rest);
        rest.erase(0, rest.find_first_not_of(' '));
        Polygon query;
        if (!parse_polygon(rest, query) || query.points.size() < 3) {
          invalid = true;
        }
        else {
            intResult = std::count_if(polygons.begin(), polygons.end(),
                std::bind(polygons_intersect, std::placeholders::_1, std::cref(query)));
        }
    }
    else if (cmd == "RMECHO") {

    }
    else invalid = true;

    if (invalid) {
        std::cout << "<INVALID COMMAND>\n";
    }
    else if (printDouble) {
        std::cout << std::fixed << std::setprecision(1) << dblResult << "\n";
    }
    else {
        std::cout << intResult << "\n";
    }
}

void process_commands(std::vector<Polygon>& polygons)
{
    std::for_each(
        std::istream_iterator<Line>(std::cin),
        std::istream_iterator<Line>(),
        [&polygons](const Line& line) {
            if (line.content.empty()) return;

            std::istringstream iss(line.content);
            std::string cmd;
            iss >> cmd;

            if (cmd == "RMECHO") {
                std::string rest;
                std::getline(iss, rest);
                rest.erase(0, rest.find_first_not_of(' '));
                Polygon query;
                if (parse_polygon(rest, query)) {
                    process_rmecho(polygons, query);
                }
                else {
                    std::cout << "<INVALID COMMAND>\n";
                }
            }
            else {
                process_command(polygons, cmd, iss);
            }
        }
    );
}

}
