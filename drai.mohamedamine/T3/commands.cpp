#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <functional>
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
                if (args.empty()) {throw std::invalid_argument("Invalid command");}

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
                    if (polygons.empty()) {throw std::invalid_argument("Invalid command");}
                    double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                        [](double acc, const Polygon& p) {
                            return acc + compute_area(p);
                        });
                    std::cout << sum / polygons.size() << '\n';
                }
                else {
                    size_t num = std::stoul(args[0]);
                    double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                        [num](double acc, const Polygon& p) {
                            return acc + (p.vertexCount() == num ? compute_area(p) : 0.0);
                        });
                    std::cout << sum << '\n';
                }
            }
            else if (command == "COUNT") {
                if (args.empty()) {throw std::invalid_argument("Invalid command");}

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
                    if (num < 3) {
                        throw std::invalid_argument("Invalid command");
                        }
                    size_t count = std::count_if(polygons.begin(), polygons.end(),
                        [num](const Polygon& p) { return p.vertexCount() == num; });
                    std::cout << count << '\n';
                }
            }
            else if (command == "MAX") {
                if (args.empty() || polygons.empty()) throw std::invalid_argument("Invalid command");

                if (args[0] == "AREA") {
                    if (polygons.empty()) throw std::invalid_argument("Invalid command");
                    auto max_it = std::max_element(polygons.begin(), polygons.end(),
                        [](const Polygon& a, const Polygon& b) {
                            return compute_area(a) < compute_area(b);
                        });
                    std::cout << compute_area(*max_it) << '\n';
                }
                else if (args[0] == "VERTEXES") {
                     if (polygons.empty()) throw std::invalid_argument("Invalid command");
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
                    if (polygons.empty()) {throw std::invalid_argument("Invalid command");}
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
                if (args.empty() || polygons.empty()) throw std::invalid_argument("Invalid command");

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
                if (args.empty() || polygons.empty()) throw std::invalid_argument("Invalid command");

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

    void process_command_group(std::vector<std::string>::iterator& it,
                             std::vector<std::string>::iterator end,
                             std::vector<Polygon>& polygons)
    {
        if (it == end) return;

        std::string command = *it++;
        std::vector<std::string> args;

        auto next_cmd = std::find_if(it, end,
            [](const std::string& s) {
                return s == "AREA" || s == "MAX" || s == "MIN" || s == "COUNT" ||
                       s == "INTERSECTIONS" || s == "RMECHO";
            });

        std::copy(it, next_cmd, std::back_inserter(args));
        it = next_cmd;

        execute_command(command, args, polygons);
    }

    void process_commands(std::vector<Polygon>& polygons)
    {
        if (std::cin.peek() == EOF) {
        std::cout << "Atleast 2 optional supported commands\n";
        return;
    }
        std::vector<std::string> commands;
        std::copy(std::istream_iterator<std::string>(std::cin),
                  std::istream_iterator<std::string>(),
                  std::back_inserter(commands));

        auto it = commands.begin();

        std::function<void(std::vector<std::string>::iterator&)> process_all =
            [&](std::vector<std::string>::iterator& iter)
        {
            if (iter == commands.end()) return;
            process_command_group(iter, commands.end(), polygons);
            process_all(iter);
        };

        process_all(it);
    }
}
