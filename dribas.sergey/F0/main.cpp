#include "workout.hpp"
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[])
{
  if (argc < 2) {
    std::cerr << "<gpx_file_name>" << '\n';
    return 1;
  }

  std::string filePath = argv[1];
  std::ifstream gpxFile(filePath);

  if (!gpxFile.is_open()) {
    std::cerr << "Error" << filePath << '\n';
    return 1;
  }

  dribas::workout myWorkout;
  gpxFile >> myWorkout;

  gpxFile.close();

  if (myWorkout.name == "Unknown workout" && myWorkout.timeStart == 0) {
    std::cerr << "Parsing failed." << '\n';
    return 1;
  }
  std::cout << myWorkout;

  return 0;
}
