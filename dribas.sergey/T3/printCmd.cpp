#include "printCmd.hpp"
#include <map>
#include <functional>
#include <algorithm>
#include <numeric>
#include <iostream>

namespace
{
  bool isEven(const dribas::Poligon& plg)
  {
    return plg.points.size() % 2;
  }

  bool isnEven(const dribas::Poligon& plg)
  {
    return !isEven(plg);
  }

  bool isNumofVErtex(const dribas::Poligon& plg, size_t vertx)
  {
    return plg.points.size() == vertx;
  }

  void printAreaPrediacte(const std::vector< dribas::Poligon >& plg, std::ostream& out, std::function< bool(const dribas::Poligon&) > predicate)
  {
    std::vector< dribas::Poligon > temp;
    std::copy_if(plg.cbegin(), plg.cend(), std::back_inserter(temp), predicate);

    std::vector< double > areas;
    std::transform(temp.begin(), temp.end(), std::back_inserter(areas), dribas::getPoligonArea);

    out <<  std::accumulate(areas.begin(), areas.end(), 0);
  }

  void printMinMaxVertxe(const std::vector< dribas::Poligon >& plg, std::ostream& out, std::function< bool(const dribas::Poligon&) > compare)
  {
    auto it = *std::max_element(plg.begin(), plg.end(), compare);
    out << it.points.size();
  }

  void printMinMaxVertxe(const std::vector< dribas::Poligon >& plg, std::ostream& out, std::function< bool(const dribas::Poligon&) > compare)
  {
    auto it = *std::max_element(plg.begin(), plg.end(), compare);
    out << dribas::getPoligonArea(it);
  }

  // bool getMinЬфчArea(dribas::Poligon& plg, std::ostream& out, std::istream& in, bool Compare)
  // {
  //   return 
  // }

  void printNumofVertex(const std::vector< dribas::Poligon >& plg, std::istream& in, std::ostream& out)
  {
    size_t vertex = 0;
    if (!(in >> vertex)) {
      throw std::invalid_argument("Invalid vertex count input");
    }
    printAreaPrediacte(plg, out, std::bind(isNumofVErtex, std::placeholders::_1, vertex));
  }


}

namespace dribas
{
  void printArea(const std::vector< Poligon >& plg, std::istream& in, std::ostream& out)
  {
    if (!plg.size()) {
      throw std::out_of_range("No figure found");
    }
    std::string command;
    std::map< std::string, std::function< void() > > cmds;
    cmds["EVEN"] = std::bind(printAreaPrediacte, std::cref(plg), std::ref(out), std::cref(isEven));
    cmds["ODD"] = std::bind(printAreaPrediacte, std::cref(plg), std::ref(out), std::cref(isnEven));
    cmds["<num-of-vertexes>"] = std::bind(printNumofVertex, std::cref(plg), std::ref(in), std::ref(out));
  }

  void printMax(const std::vector< Poligon >&, std::istream&, std::ostream&)
  {

  }
}