#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <vector>
#include "polygon.hpp"

namespace amine
{
  struct AreaEvenAccumulator
  {
    double operator()(double acc, const Polygon& p) const
    {
      return acc + ((p.points.size() % 2 == 0) ? compute_area(p) : 0.0);
    }
  };

  struct AreaOddAccumulator
  {
    double operator()(double acc, const Polygon& p) const
    {
      return acc + ((p.points.size() % 2 != 0) ? compute_area(p) : 0.0);
    }
  };

  struct AreaMeanAccumulator
  {
    double operator()(double acc, const Polygon& p) const
    {
      return acc + compute_area(p);
    }
  };

  struct AreaNumAccumulator
  {
    int num;
    explicit AreaNumAccumulator(int n):
      num(n)
    {}
    double operator()(double acc, const Polygon& p) const
    {
      return acc + (static_cast< int >(p.points.size()) == num ? compute_area(p) : 0.0);
    }
  };

  struct CountEven
  {
    bool operator()(const Polygon& p) const
    {
      return p.points.size() % 2 == 0;
    }
  };

  struct CountOdd
  {
    bool operator()(const Polygon& p) const
    {
      return p.points.size() % 2 != 0;
    }
  };

  struct CountNum
  {
    int num;
    explicit CountNum(int n):
      num(n)
    {}
    bool operator()(const Polygon& p) const
    {
      return static_cast< int >(p.points.size()) == num;
    }
  };

  struct AreaComparator
  {
    bool operator()(const Polygon& a, const Polygon& b) const
    {
      return compute_area(a) < compute_area(b);
    }
  };

  struct VertexesComparator
  {
    bool operator()(const Polygon& a, const Polygon& b) const
    {
      return a.points.size() < b.points.size();
    }
  };
  std::istream& operator>>(std::istream& is, Line& line)
  {
    std::getline(is, line.content);
    return is;
  }

struct PointEqual {
    bool operator()(const Point& a, const Point& b) const {
        return a.x == b.x && a.y == b.y;
    }
};
struct PolygonEqual {
    const Polygon& query;
    explicit PolygonEqual(const Polygon& q) : query(q) {}

    bool operator()(const Polygon& p) const {
        return p.points.size() == query.points.size() &&
               std::equal(p.points.begin(), p.points.end(), query.points.begin(), PointEqual());
    }
};
struct BothEqualQuery {
    PolygonEqual eq;
    explicit BothEqualQuery(const Polygon& q) : eq(q) {}

    bool operator()(const Polygon& a, const Polygon& b) const {
        return eq(a) && eq(b);
    }
};
void process_rmecho(std::vector<Polygon>& polygons, const Polygon& query) {
    size_t initial_size = polygons.size();
    auto new_end = std::unique(polygons.begin(), polygons.end(), BothEqualQuery(query));
    polygons.erase(new_end, polygons.end());
    std::cout << (initial_size - polygons.size()) << "\n";
}
struct IntersectsWith {
    const Polygon& query;
    explicit IntersectsWith(const Polygon& q) : query(q) {}
    bool operator()(const Polygon& p) const {
        return polygons_intersect(p, query);
    }
};

struct CommandHandler {
    std::vector<Polygon>& polygons;

    explicit CommandHandler(std::vector<Polygon>& polys) : polygons(polys) {}

    void operator()(const Line& line) const {
        if (line.content.empty()) return;

        std::istringstream iss(line.content);
        std::string cmd;
        iss >> cmd;
        bool invalid = false;
        bool printDouble = false;
        double dblResult = 0.0;
        int intResult = 0;

        if (cmd == "AREA") {
            std::string arg;
            iss >> arg;
            if (arg == "EVEN") {
                dblResult = std::accumulate(polygons.begin(), polygons.end(), 0.0, AreaEvenAccumulator());
                printDouble = true;
            }
            else if (arg == "ODD") {
                dblResult = std::accumulate(polygons.begin(), polygons.end(), 0.0, AreaOddAccumulator());
                printDouble = true;
            }
            else if (arg == "MEAN") {
                if (polygons.empty()) {
                    invalid = true;
                }
                else {
                    dblResult = std::accumulate(polygons.begin(), polygons.end(), 0.0, AreaMeanAccumulator()) / polygons.size();
                    printDouble = true;
                }
            }
            else {
                try {
                    int num = std::stoi(arg);
                    if (num >= 3) {
                        dblResult = std::accumulate(polygons.begin(), polygons.end(), 0.0, AreaNumAccumulator(num));
                        printDouble = true;
                    }
                    else {
                        invalid = true;
                    }
                }
                catch (...) {
                    invalid = true;
                }
            }
        }
        else if (cmd == "INTERSECTIONS") {
            std::string rest;
            std::getline(iss, rest);
            Polygon query;
            if (parse_polygon(rest, query)) {
                intResult = std::count_if(polygons.begin(), polygons.end(), IntersectsWith(query));
            }
            else {
                invalid = true;
            }
        }
        else if (cmd == "RMECHO") {
            std::string rest;
            std::getline(iss, rest);
            Polygon query;
            if (parse_polygon(rest, query)) {
                size_t initial_size = polygons.size();
                auto new_end = std::unique(polygons.begin(), polygons.end(), BothEqualQuery(query));
                polygons.erase(new_end, polygons.end());
                std::cout << (initial_size - polygons.size()) << "\n";
                return;
            }
            else {
                invalid = true;
            }
        }
        else {
            invalid = true;
        }

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
};

void process_commands(std::vector<Polygon>& polygons) {
    std::for_each(
        std::istream_iterator<Line>(std::cin),
        std::istream_iterator<Line>(),
        CommandHandler(polygons)
    );
}

}
