#ifndef WORKOUT_HPP
#define WORKOUT_HPP

#include <string>
#include <ctime>
#include <fstream>
#include <cmath>
#include <istream>
#include <ostream>

namespace dribas
{
  struct workout
  {
    std::string name;
    int avgHeart = 0;
    int maxHeart = 0;
    int cadence = 0;
    double distance = 0.0;
    double avgPaceMinPerKm = 0.0;
    time_t timeStart = 0;
    time_t timeEnd = 0;
  };

  workout parseGpx(const std::string& gpxContent);

  std::istream& operator>>(std::istream& is, workout& w);
  std::ostream& operator<<(std::ostream& os, const workout& w);
}

#endif
