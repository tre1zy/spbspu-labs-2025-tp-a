#include "commands.hpp"
#include "polygon.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <functional>
#include <optional>

namespace amine
{
  namespace
  {
    using namespace std;

    bool is_number(const string& s)
    {
      return !s.empty() && all_of(s.begin(), s.end(), ::isdigit);
    }

    void print_invalid()
    {
      cout << "<INVALID COMMAND>" << '\n';
    }

    void print_polygon_error()
    {
      cerr << "Invalid polygon format" << '\n';
    }

    void echo(const vector<string>& args, vector<Polygon>& polys)
    {
      auto p = parse_polygon(args);
      if (!p) {
        print_polygon_error();
        return;
      }
      polys.push_back(*p);
    }

    void count_cmd(const vector<string>& args, const vector<Polygon>& polys)
    {
      if (args.size() != 2) {
        print_invalid();
        return;
      }

      string mode = args[1];
      if (mode == "EVEN") {
        cout << count_if(polys.begin(), polys.end(), [](const Polygon& p) {
          return p.vertexCount() % 2 == 0;
        }) << '\n';
      }
      else if (mode == "ODD") {
        cout << count_if(polys.begin(), polys.end(), [](const Polygon& p) {
          return p.vertexCount() % 2 != 0;
        }) << '\n';
      }
      else if (is_number(mode)) {
        size_t target = stoi(mode);
        cout << count_if(polys.begin(), polys.end(), [&](const Polygon& p) {
          return p.vertexCount() == target;
        }) << '\n';
      }
      else print_invalid();
    }

    void area_cmd(const vector<string>& args, const vector<Polygon>& polys)
    {
      if (args.size() != 2) {
        print_invalid();
        return;
      }

      string mode = args[1];
      if (mode == "EVEN" || mode == "ODD") {
        bool even = (mode == "EVEN");
        double sum = accumulate(polys.begin(), polys.end(), 0.0, [even](double acc, const Polygon& p) {
          return acc + ((p.vertexCount() % 2 == 0) == even ? p.area() : 0.0);
        });
        cout << fixed << setprecision(1) << sum << '\n';
      }
      else if (mode == "MEAN") {
        if (polys.empty()) {
          print_invalid();
          return;
        }
        double total = accumulate(polys.begin(), polys.end(), 0.0, [](double acc, const Polygon& p) {
          return acc + p.area();
        });
        cout << fixed << setprecision(1) << total / polys.size() << '\n';
      }
      else if (is_number(mode)) {
        size_t target = stoi(mode);
        double sum = accumulate(polys.begin(), polys.end(), 0.0, [=](double acc, const Polygon& p) {
          return acc + (p.vertexCount() == target ? p.area() : 0.0);
        });
        cout << fixed << setprecision(1) << sum << '\n';
      }
      else print_invalid();
    }

    void max_cmd(const vector<string>& args, const vector<Polygon>& polys)
    {
      if (args.size() != 2 || polys.empty()) {
        print_invalid();
        return;
      }

      if (args[1] == "AREA") {
        auto max_it = max_element(polys.begin(), polys.end(), [](const Polygon& a, const Polygon& b) {
          return a.area() < b.area();
        });
        cout << fixed << setprecision(1) << max_it->area() << '\n';
      }
      else if (args[1] == "VERTEXES") {
        auto max_it = max_element(polys.begin(), polys.end(), [](const Polygon& a, const Polygon& b) {
          return a.vertexCount() < b.vertexCount();
        });
        cout << max_it->vertexCount() << '\n';
      }
      else print_invalid();
    }

    void same_cmd(const vector<string>& args, const vector<Polygon>& polys)
    {
      auto p = parse_polygon(args);
      if (!p) {
        print_invalid();
        return;
      }

      size_t same = count_if(polys.begin(), polys.end(), [&](const Polygon& other) {
        return other == *p;
      });
      cout << same << '\n';
    }

    void process_line(const string& line, vector<Polygon>& polys)
    {
      istringstream ss(line);
      vector<string> tokens{istream_iterator<string>{ss}, istream_iterator<string>{}};

      if (tokens.empty()) return;
      const string& cmd = tokens[0];

      if (cmd == "ECHO") echo(tokens, polys);
      else if (cmd == "COUNT") count_cmd(tokens, polys);
      else if (cmd == "AREA") area_cmd(tokens, polys);
      else if (cmd == "MAX") max_cmd(tokens, polys);
      else if (cmd == "SAME") same_cmd(tokens, polys);
      else print_invalid();
    }
  }

  optional<Polygon> parse_polygon(const vector<string>& args)
  {
    if (args.size() < 2 || !is_number(args[1])) {
      return nullopt;
    }
    size_t n = stoi(args[1]);
    if (args.size() != 2 + n) {
      return nullopt;
    }

    try {
      vector<Point> pts(n);
      transform(args.begin() + 2, args.end(), pts.begin(), [](const string& s) {
        auto l = s.find('('), m = s.find(';'), r = s.find(')');
        if (l == string::npos || m == string::npos || r == string::npos) throw invalid_argument("invalid");
        double x = stod(s.substr(l + 1, m - l - 1));
        double y = stod(s.substr(m + 1, r - m - 1));
        return Point{x, y};
      });
      return Polygon{pts};
    } catch (...) {
      return nullopt;
    }
  }

 void process_commands(vector<Polygon>& polygons)
{
    istreambuf_iterator<char> begin(cin), end;
    string input(begin, end);

    vector<string> lines;
    istringstream iss(input);

    size_t line_count = count(input.begin(), input.end(), '\n') + 1;

    lines.reserve(line_count);

    generate_n(back_inserter(lines), line_count, [&iss]() {
        string line;
        getline(iss, line);
        return line;
    });

    for_each(lines.begin(), lines.end(), [&polygons](const string& line) {
        if (!line.empty()) {
            process_line(line, polygons);
        }
    });
}
}
