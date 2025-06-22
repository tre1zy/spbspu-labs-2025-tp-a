#include "data_input.hpp"
#include <iterator>
#include <algorithm>
#include <string>
#include <iostream>
#include <sstream>

std::istream& trukhanov::operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) return in;

  char c = '\0';
  in >> c;
  if (in && c != dest.exp)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& trukhanov::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry sentry(in);
  if (!sentry) return in;

  in >> DelimiterIO{ '(' } >> point.x >> DelimiterIO{ ';' } >> point.y >> DelimiterIO{ ')' };
  return in;
}

std::ostream& trukhanov::operator<<(std::ostream& out, const Point& point)
{
  out << '(' << point.x << ';' << point.y << ')';
  return out;
}

std::istream& trukhanov::operator>>(std::istream& in, Polygon& polygon)
{
  polygon.points.clear();

  std::string line;
  if (!std::getline(in, line))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  std::istringstream iss(line);

  size_t n = 0;
  iss >> n;
  if (!iss || n < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  Polygon temp;
  temp.points.reserve(n);
  std::copy_n(std::istream_iterator<Point>(iss), n, std::back_inserter(temp.points));

  Point extra;
  if (iss >> extra)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  polygon = std::move(temp);
  return in;
}

std::ostream& trukhanov::operator<<(std::ostream& out, const Polygon& polygon)
{
  out << polygon.points.size() << ' ';
  std::copy(polygon.points.begin(), polygon.points.end(), std::ostream_iterator<Point>(out, " "));
  return out;
}
