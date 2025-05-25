#include <vector>
#include <iostream>

namespace klimova {
    struct Point {
        int x, y;
    };
    struct DelimiterIO {
        char exp;
    };

    std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
    std::istream& operator>>(std::istream& in, Point& point);
    std::ostream& operator<<(std::ostream& out, const Point& point);

    struct Polygon {
        std::vector< Point > points;
    };

    std::istream& operator>>(std::istream& in, Polygon& poly);
    std::ostream& operator<<(std::ostream& out, const Polygon& polygon);
    bool isEven(const Polygon& p);
    bool isOdd(const Polygon& p);
    double polygonArea(const Polygon& poly);
}
