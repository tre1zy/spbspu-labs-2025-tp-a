#include "commands.hpp"
#include "streamguard.hpp"
#include "utils.hpp"
#include <algorithm>
#include <functional>
#include <numeric>
#include <iomanip>
#include <string>

void klimova::area(const std::vector< Polygon >& polygons, std::istream& is, std::ostream& os)
{
    auto bindEven = std::bind(areaEven, _1, _2, polygons.size());
    auto bindOdd = std::bind(areaOdd, _1, _2, polygons.size());
    auto bindMean = std::bind(areaMean, _1, _2, polygons.size());
    AreaSubcommands subs{{"EVEN", bindEven}, {"ODD", bindOdd}, {"MEAN", bindMean}};

    std::string subcommand;
    is >> subcommand;
    double result = 0.0;
    try {
        if (subs.find(subcommand) != subs.end()) {
            result = std::accumulate(polygons.begin(), polygons.end(), 0.0, subs.at(subcommand));
        } else {
            size_t vertexes = getVertexes(subcommand);
            result = std::accumulate(polygons.begin(), polygons.end(), 0.0, std::bind(areaNumber, _1, _2, vertexes));
        }
        Streamguard StreamGuard(os);
        os << std::fixed << std::setprecision(1) << result << "\n";
    }
    catch (const std::exception&) {
        os << "<INVALID COMMAND>\n";
    }
}

void klimova::max(const std::vector< Polygon >& polygons, std::istream& is, std::ostream& os) {
    MaxSubcommands subs{{"AREA", getArea}, {"VERTEXES", getVertexesCount}};

    std::string subcommand;
    is >> subcommand;
    try {
        auto func = subs.at(subcommand);
        auto compare = std::bind(std::less<double>(), std::bind(func, _1), std::bind(func, _2));
        auto it = std::max_element(polygons.begin(), polygons.end(), compare);
        Streamguard StreamGuard(os);
        os << std::fixed << std::setprecision(1);
        if (subcommand == "AREA") {
            os << func(*it) << "\n";
        } else {
            os << static_cast<int>(func(*it)) << "\n";
        }
    }
    catch (const std::out_of_range&) {
        os << "<INVALID COMMAND>\n";
    }
}

void klimova::min(const std::vector< Polygon >& polygons, std::istream& is, std::ostream& os) {
    MinSubcommands subs{{"AREA", getArea}, {"VERTEXES", getVertexesCount}};

    std::string subcommand;
    is >> subcommand;
    try {
        auto func = subs.at(subcommand);
        auto compare = std::bind(std::less<double>(), std::bind(func, _1), std::bind(func, _2));
        auto it = std::min_element(polygons.begin(), polygons.end(), compare);
        Streamguard StreamGuard(os);
        os << std::fixed << std::setprecision(1);
        if (subcommand == "AREA") {
            os << func(*it) << "\n";
        } else {
            os << static_cast<int>(func(*it)) << "\n";
        }
    }
    catch (const std::out_of_range&) {
        os << "<INVALID COMMAND>\n";
    }
}

void klimova::count(const std::vector< Polygon >& polygons, std::istream& is, std::ostream& os) {
    auto bindEven = std::bind(isVertexCountEven, _1);
    auto bindOdd = std::bind(isVertexCountOdd, _1);
    CountSubcommands subs{{"EVEN", bindEven}, {"ODD", bindOdd}};

    std::string subcommand;
    is >> subcommand;
    try {
        size_t result = 0;
        if (subs.find(subcommand) != subs.end()) {
            auto predicate = subs.at(subcommand);
            result = std::count_if(polygons.begin(), polygons.end(), predicate);
        } else {
            size_t vertexes = getVertexes(subcommand);
            if (!isValidVertexCount(vertexes)) {
                os << "<INVALID COMMAND>\n";
                return;
            }
            auto predicate = std::bind(hasVertexCount, _1, vertexes);
            result = std::count_if(polygons.begin(), polygons.end(), predicate);
        }
        os << result << "\n";
    }
    catch (const std::exception&) {
        os << "<INVALID COMMAND>\n";
    }
}
