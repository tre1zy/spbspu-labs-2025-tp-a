#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include "polygon.hpp"
#include "commands.hpp"

namespace amine
{
    void execute_command(const std::string& command, const std::vector<std::string>& args, std::vector<Polygon>& polygons)
    {
        std::cout << std::fixed << std::setprecision(1);

        try {
            if (command == "AREA") {
                if (args.empty()) throw std::invalid_argument("Invalid command");

                if (args[0] == "EVEN") {
                    double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                        [](double acc, const Polygon& p) {
                            return acc + (p.vertexCount() % 2 == 0 ? compute_area(p) : 0.0);
                        });
                    std::cout << sum << '\n';
                }
                else if (args[0] == "ODD") {
                    double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                        [](double acc, const Polygon& p) {
                            return acc + (p.vertexCount() % 2 != 0 ? compute_area(p) : 0.0);
                        });
                    std::cout << sum << '\n';
                }
                else if (args[0] == "MEAN") {
                    if (polygons.empty()) throw std::invalid_argument("Invalid command");
                    double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                        [](double acc, const Polygon& p) {
                            return acc + compute_area(p);
                        });
                    std::cout << sum / polygons.size() << '\n';
                }
                else {
                    size_t num = std::stoul(args[0]);
                    if (num < 3) throw std::invalid_argument("Invalid command");
                    double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                        [num](double acc, const Polygon& p) {
                            return acc + (p.vertexCount() == num ? compute_area(p) : 0.0);
                        });
                    std::cout << sum << '\n';
                }
            }
            else if (command == "COUNT") {
                if (args.empty()) throw std::invalid_argument("Invalid command");

                if (args[0] == "EVEN") {
                    size_t count = std::count_if(polygons.begin(), polygons.end(),
                        [](const Polygon& p) { return p.vertexCount() % 2 == 0; });
                    std::cout << count << '\n';
                }
                else if (args[0] == "ODD") {
                    size_t count = std::count_if(polygons.begin(), polygons.end(),
                        [](const Polygon& p) { return p.vertexCount() % 2 != 0; });
                    std::cout << count << '\n';
                }
                else {
                    size_t num = std::stoul(args[0]);
                    if (num < 3) throw std::invalid_argument("Invalid command");
                    size_t count = std::count_if(polygons.begin(), polygons.end(),
                        [num](const Polygon& p) { return p.vertexCount() == num; });
                    std::cout << count << '\n';
                }
            }
            else if (command == "MAX") {
                if (args.empty() || polygons.empty()) throw std::invalid_argument("Invalid command");

                if (args[0] == "AREA") {
                    auto max_it = std::max_element(polygons.begin(), polygons.end(),
                        [](const Polygon& a, const Polygon& b) {
                            return compute_area(a) < compute_area(b);
                        });
                    std::cout << compute_area(*max_it) << '\n';
                }
                else if (args[0] == "VERTEXES") {
                    auto max_it = std::max_element(polygons.begin(), polygons.end(),
                        [](const Polygon& a, const Polygon& b) {
                            return a.vertexCount() < b.vertexCount();
                        });
                    std::cout << max_it->vertexCount() << '\n';
                }
                else {
                    throw std::invalid_argument("Invalid command");
                }
            }
            else if (command == "MIN") {
                if (args.empty() || polygons.empty()) throw std::invalid_argument("Invalid command");

                if (args[0] == "AREA") {
                    auto min_it = std::min_element(polygons.begin(), polygons.end(),
                        [](const Polygon& a, const Polygon& b) {
                            return compute_area(a) < compute_area(b);
                        });
                    std::cout << compute_area(*min_it) << '\n';
                }
                else if (args[0] == "VERTEXES") {
                    auto min_it = std::min_element(polygons.begin(), polygons.end(),
                        [](const Polygon& a, const Polygon& b) {
                            return a.vertexCount() < b.vertexCount();
                        });
                    std::cout << min_it->vertexCount() << '\n';
                }
                else {
                    throw std::invalid_argument("Invalid command");
                }
            }
            else if (command == "INTERSECTIONS") {
                if (args.empty()) throw std::invalid_argument("Invalid command");

                Polygon param;
                std::string poly_str = std::accumulate(args.begin(), args.end(), std::string(),
                    [](const std::string& a, const std::string& b) {
                        return a + (a.empty() ? "" : " ") + b;
                    });

                if (!parse_polygon(poly_str, param)) {
                    throw std::invalid_argument("Invalid command");
                }

                int count = std::count_if(polygons.begin(), polygons.end(),
                    [&param](const Polygon& p) {
                        return polygons_intersect(p, param);
                    });

                std::cout << count << '\n';
            }
            else if (command == "RMECHO") {
                if (args.empty()) throw std::invalid_argument("Invalid command");

                Polygon param;
                std::string poly_str = std::accumulate(args.begin(), args.end(), std::string(),
                    [](const std::string& a, const std::string& b) {
                        return a + (a.empty() ? "" : " ") + b;
                    });

                if (!parse_polygon(poly_str, param)) {
                    throw std::invalid_argument("Invalid command");
                }

                auto new_end = std::unique(polygons.begin(), polygons.end(),
                    [&param](const Polygon& a, const Polygon& b) {
                        return a == param && b == param;
                    });
                int removed = std::distance(new_end, polygons.end());
                polygons.erase(new_end, polygons.end());
                std::cout << removed << '\n';
            }
            else {
                throw std::invalid_argument("Invalid command");
            }
        }
        catch (...) {
            std::cout << "<INVALID COMMAND>\n";
        }
    }

    void process_commands(std::vector<Polygon>& polygons)
    {
        std::string line;
        while (std::getline(std::cin, line)) {
            if (line.empty()) continue;

            std::istringstream iss(line);
            std::vector<std::string> tokens;
            std::copy(std::istream_iterator<std::string>(iss),
                     std::istream_iterator<std::string>(),
                     std::back_inserter(tokens));

            if (tokens.empty()) continue;

            std::string command = tokens[0];
            std::vector<std::string> args(tokens.begin() + 1, tokens.end());

            execute_command(command, args, polygons);
        }
    }
}
