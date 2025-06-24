#include <sstream>
#include <iomanip>
#include "polygon.hpp"

bool Polygon::operator==(const Polygon & other) const {
  return points == other.points;
}

bool Polygon::operator<(const Polygon & other) const {
  return points < other.points;
}

size_t Polygon::vertexCount() const {
  return points.size();
}

double Polygon::area() const {
  double a = 0.0;
  size_t n = points.size();
  for (size_t i = 0; i < n; ++i) {
    const Point & p1 = points[i];
    const Point & p2 = points[(i + 1) % n];
    a += static_cast<double>(p1.x) * p2.y - static_cast<double>(p2.x) * p1.y;
  }
  a = a < 0 ? -a : a;
  return a * 0.5;
}

std::ostream & operator<<(std::ostream & os, const Polygon & poly) {
  os << poly.points.size();
  for (size_t i = 0; i < poly.points.size(); ++i) {
    os << " " << poly.points[i];
  }
  return os;
}

std::istream & operator>>(std::istream & is, Polygon & poly) {
  poly.points.clear();
  size_t n = 0;
  is >> n;
  if (!is || n == 0) {
    is.setstate(std::ios::failbit);
    return is;
  }
  poly.points.resize(n);
  for (size_t i = 0; i < n; ++i) {
    if (!(is >> poly.points[i])) {
      is.setstate(std::ios::failbit);
      return is;
    }
  }
  return is;
}

#include <regex>

bool parsePolygon(const std::string & line, Polygon & poly) {

  std::istringstream iss(line);
  size_t n = 0;
  iss >> n;
  if (!iss || n == 0)
    return false;

  std::vector<Point> pts;
  pts.reserve(n);

  for (size_t i = 0; i < n; ++i) {
    Point p;
    if (!(iss >> p)) {
      return false;
    }
    pts.push_back(p);
  }

  std::string rem;
  if (iss >> rem) {
    return false;
  }

  poly.points = std::move(pts);
  return true;
}
