#include "commands.hpp"
#include "streamguard.hpp"
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
        subs.at(subcommand)(0.0, *polygons.begin());
        result = std::accumulate(polygons.begin(), polygons.end(), 0.0, subs[subcommand]);
    }
    catch (const std::out_of_range& e) {
        size_t vertexes = getVertexes(subcommand);
        result = std::accumulate(polygons.begin(), polygons.end(), 0.0, std::bind(areaNumber, _1, _2, vertexes));
    }
    Streamguard StreamGuard(os);
    os << std::fixed << std::setprecision(1) << result << "\n";
}
