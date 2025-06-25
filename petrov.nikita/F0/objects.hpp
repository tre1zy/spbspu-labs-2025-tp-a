#ifndef OBJECTS_HPP
#define OBJECTS_HPP

#include <string>
#include <map>
#include <vector>

namespace petrov
{
  struct Planet
  {
    double x, y;
  };

  struct Map
  {
    Map() = default;
    void addPlanet(const Planet & planet);
    double findOptimalWay(std::string & first_planet_name, std::string & second_planet_name);
  private:
    std::map< std::string, Planet > planets_;
    std::vector< std::vector< double > > adjency_matrix_;
  };
}

#endif
