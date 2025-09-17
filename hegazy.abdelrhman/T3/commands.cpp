#include "commands.hpp"
#include <algorithm>
#include <iomanip>
#include <limits>
#include <numeric>
#include <functional>
#include <unordered_map>
#include <string>
#include <set>
#include "geometry.hpp"
#include <stdexcept>

namespace bob
{
    Iofmtguard::Iofmtguard(std::ostream& os) :
        os_(os),
        flags_(os.flags()),
        fill_(os.fill())
    {
    }

    Iofmtguard::~Iofmtguard()
    {
        os_.flags(flags_);
        os_.fill(fill_);
    }
}

namespace
{
    using geom::Polygon;

    bool hasEvenVertices(const Polygon& poly)
    {
        return poly.points.size() % 2 == 0;
    }

    bool hasOddVertices(const Polygon& poly)
    {
        return !hasEvenVertices(poly);
    }

    struct VertexCountMatcher
    {
        size_t targetCount;
        bool operator()(const Polygon& poly) const
        {
            return poly.points.size() == targetCount;
        }
    };

    size_t countEvenVertices(const std::vector<Polygon>& polygons)
    {
        return std::count_if(polygons.begin(), polygons.end(), hasEvenVertices);
    }

    size_t countOddVertices(const std::vector<Polygon>& polygons)
    {
        return std::count_if(polygons.begin(), polygons.end(), hasOddVertices);
    }

    size_t countByVertexCount(const std::vector<Polygon>& polygons, size_t count)
    {
        return std::count_if(polygons.begin(), polygons.end(), VertexCountMatcher{ count });
    }

    bool compareByVertexCount(const Polygon& lhs, const Polygon& rhs)
    {
        return lhs.points.size() < rhs.points.size();
    }

    bool compareByArea(const Polygon& lhs, const Polygon& rhs)
    {
        return geom::getPolygonArea(lhs) < geom::getPolygonArea(rhs);
    }

    double totalEvenArea(const std::vector<Polygon>& polygons)
    {
        double sum = 0.0;
        for (const auto& poly : polygons) {
            if (hasEvenVertices(poly)) {
                sum += geom::getPolygonArea(poly);
            }
        }
        return sum;
    }

    double totalOddArea(const std::vector<Polygon>& polygons)
    {
        double sum = 0.0;
        for (const auto& poly : polygons) {
            if (hasOddVertices(poly)) {
                sum += geom::getPolygonArea(poly);
            }
        }
        return sum;
    }

    double totalAreaByVertexCount(const std::vector<Polygon>& polygons, size_t count)
    {
        double sum = 0.0;
        for (const auto& poly : polygons) {
            if (poly.points.size() == count) {
                sum += geom::getPolygonArea(poly);
            }
        }
        return sum;
    }

    double totalAreaAll(const std::vector<Polygon>& polygons)
    {
        double sum = 0.0;
        for (const auto& poly : polygons) {
            sum += geom::getPolygonArea(poly);
        }
        return sum;
    }

    struct AverageAreaCalculator
    {
        double operator()(const std::vector<Polygon>& polygons) const
        {
            if (polygons.empty()) {
                throw std::invalid_argument("No polygons");
            }
            return totalAreaAll(polygons) / polygons.size();
        }
    };

    void outputMaxVertices(std::ostream& os, const std::vector<Polygon>& polygons)
    {
        if (polygons.empty()) return;
        auto max_poly = *std::max_element(polygons.begin(), polygons.end(), compareByVertexCount);
        os << max_poly.points.size();
    }

    void outputMaxArea(std::ostream& os, const std::vector<Polygon>& polygons)
    {
        if (polygons.empty()) return;
        auto max_poly = *std::max_element(polygons.begin(), polygons.end(), compareByArea);
        os << std::fixed << std::setprecision(1) << geom::getPolygonArea(max_poly);
    }

    void outputMinVertices(std::ostream& os, const std::vector<Polygon>& polygons)
    {
        if (polygons.empty()) return;
        auto min_poly = *std::min_element(polygons.begin(), polygons.end(), compareByVertexCount);
        os << min_poly.points.size();
    }

    void outputMinArea(std::ostream& os, const std::vector<Polygon>& polygons)
    {
        if (polygons.empty()) return;
        auto min_poly = *std::min_element(polygons.begin(), polygons.end(), compareByArea);
        os << std::fixed << std::setprecision(1) << geom::getPolygonArea(min_poly);
    }
}

void bob::printAreaSum(std::istream& input, const std::vector<Polygon>& polygons, std::ostream& output)
{
    using HandlerMap = std::unordered_map<std::string, std::function<double()>>;
    HandlerMap handlers;
    handlers["EVEN"] = [&]() { return totalEvenArea(polygons); };
    handlers["ODD"] = [&]() { return totalOddArea(polygons); };
    handlers["MEAN"] = [&]() { return AverageAreaCalculator{}(polygons); };

    std::string param;
    input >> param;

    double result{};
    try {
        result = handlers.at(param)();
    }
    catch (const std::out_of_range&) {
        try {
            size_t vertices = std::stoull(param);
            if (vertices < 3) {
                throw std::invalid_argument("Vertex count too small");
            }
            result = totalAreaByVertexCount(polygons, vertices);
        }
        catch (const std::invalid_argument&) {
            throw std::invalid_argument("Invalid parameter");
        }
    }

    bob::Iofmtguard guard(output);
    output << std::fixed << std::setprecision(1) << result;
}

void bob::printMaxValueOf(std::istream& input, const std::vector<Polygon>& polygons, std::ostream& output)
{
    std::string param;
    input >> param;

    if (polygons.empty()) {
        throw std::invalid_argument("No polygons available");
    }

    using HandlerMap = std::unordered_map<std::string, std::function<void()>>;
    HandlerMap handlers;
    handlers["AREA"] = [&]() { outputMaxArea(output, polygons); };
    handlers["VERTEXES"] = [&]() { outputMaxVertices(output, polygons); };

    bob::Iofmtguard guard(output);
    handlers.at(param)();
}

void bob::printMinValueOf(std::istream& input, const std::vector<Polygon>& polygons, std::ostream& output)
{
    std::string param;
    input >> param;

    if (polygons.empty()) {
        throw std::invalid_argument("No polygons available");
    }

    using HandlerMap = std::unordered_map<std::string, std::function<void()>>;
    HandlerMap handlers;
    handlers["AREA"] = [&]() { outputMinArea(output, polygons); };
    handlers["VERTEXES"] = [&]() { outputMinVertices(output, polygons); };

    bob::Iofmtguard guard(output);
    handlers.at(param)();
}

void bob::printCountOf(std::istream& input, const std::vector<Polygon>& polygons, std::ostream& output)
{
    using HandlerMap = std::unordered_map<std::string, std::function<size_t()>>;
    HandlerMap handlers;
    handlers["EVEN"] = [&]() { return countEvenVertices(polygons); };
    handlers["ODD"] = [&]() { return countOddVertices(polygons); };

    std::string param;
    input >> param;

    size_t count{};
    try {
        count = handlers.at(param)();
    }
    catch (const std::out_of_range&) {
        try {
            size_t vertices = std::stoull(param);
            if (vertices < 3) {
                throw std::invalid_argument("Invalid vertex count parameter");
            }
            count = countByVertexCount(polygons, vertices);
        }
        catch (const std::invalid_argument&) {
            throw std::invalid_argument("Invalid parameter");
        }
    }

    output << count;
}

void bob::printLessAreaCnt(std::istream& input, const std::vector<Polygon>& polygons, std::ostream& output)
{
    Polygon ref;
    input >> ref;
    if (!input) {
        throw std::invalid_argument("Invalid polygon input");
    }

    double refArea = geom::getPolygonArea(ref);
    size_t count = 0;
    for (const auto& poly : polygons) {
        if (geom::getPolygonArea(poly) < refArea) {
            count++;
        }
    }
    output << count;
}

void bob::printIntersectionsCnt(std::istream& input, const std::vector<Polygon>& polygons, std::ostream& output)
{
    Polygon ref;
    input >> ref;
    if (!input) {
        throw std::invalid_argument("Invalid polygon input");
    }


    std::set<geom::Point> uniquePoints(ref.points.begin(), ref.points.end());
    if (uniquePoints.size() != ref.points.size()) {
        throw std::invalid_argument("Reference polygon has duplicate points");
    }

    size_t count = 0;
    for (const auto& poly : polygons) {
        if (geom::doPolygonsIntersect(ref, poly)) {
            count++;
        }
    }
    output << count;
}
