#include <algorithm>
#include <fstream>
#include <functional>
#include <iterator>
#include <iostream>
#include <limits>
#include <map>
#include <vector>

namespace {
  struct DelimiterIn
  {
    char exp;
  };

  std::istream& operator>>(std::istream& in, DelimiterIn&& rhs)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    char c = '0';
    in >> c;
    if (in && (c != rhs.exp)) {
      in.setstate(std::ios::failbit);
    }
    return in;
  }
}

namespace maslevtsov {
  struct Point
  {
    int x, y;
  };

  struct Polygon
  {
    std::vector< Point > points;
  };

  std::istream& operator>>(std::istream& in, Point& rhs);
  std::istream& operator>>(std::istream& in, Polygon& rhs);

  void get_area(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void get_max(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void get_min(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void count_vertexes(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void echo(std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
  void remove_echo(std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);
}

std::istream& maslevtsov::operator>>(std::istream& in, Point& rhs)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  return in >> DelimiterIn{'('} >> rhs.x >> DelimiterIn{';'} >> rhs.y >> DelimiterIn{')'};
}

std::istream& maslevtsov::operator>>(std::istream& in, Polygon& rhs)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  std::size_t vertex_num = 0;
  in >> vertex_num;
  if (vertex_num < 3) {
    in.setstate(std::ios::failbit);
  }
  std::copy_n(std::istream_iterator< Point >(in), vertex_num, std::back_inserter(rhs.points));
  if (!in || rhs.points.size() != vertex_num) {
    rhs.points.clear();
    in.setstate(std::ios::failbit);
  }
  return in;
}

int main(int argc, char** argv)
{
  using namespace maslevtsov;

  if (argc != 2) {
    std::cerr << "<INVALID PARAMETERS NUMBER>\n";
    return 1;
  }
  std::ifstream fin(argv[1]);
  if (!fin) {
    std::cerr << "<INVALID DATA FILE>\n";
    return 1;
  }
  std::vector< Polygon > polygons;
  std::map< std::string, std::function< void(std::istream&, std::ostream&) > > commands;
  commands["AREA"] = std::bind(get_area, std::cref(polygons), std::placeholders::_1, std::placeholders::_2);
  commands["MAX"] = std::bind(get_max, std::cref(polygons), std::placeholders::_1, std::placeholders::_2);
  commands["MIN"] = std::bind(get_min, std::cref(polygons), std::placeholders::_1, std::placeholders::_2);
  commands["COUNT"] = std::bind(count_vertexes, std::cref(polygons), std::placeholders::_1, std::placeholders::_2);
  commands["ECHO"] = std::bind(get_min, std::ref(polygons), std::placeholders::_1, std::placeholders::_2);
  commands["RMECHO"] = std::bind(count_vertexes, std::ref(polygons), std::placeholders::_1, std::placeholders::_2);
  std::string command;
  while (!(fin >> command).eof()) {
    try {
      commands.at(command)(fin, std::cout);
    } catch (...) {
      if (fin.fail()) {
        fin.clear(fin.rdstate() ^ std::ios::failbit);
      }
      fin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}
