#include "race_predictor.hpp"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <numeric>
#include <vector>
#include <limits>
#include <streamGuard.hpp>


namespace
{
  struct WorkoutStats
  {
    size_t count;
    double max_distance;
    double min_pace;

    WorkoutStats():
      count(0), 
      max_distance(0.0), 
      min_pace(std::numeric_limits< double >::max())
    {}
    WorkoutStats operator()(WorkoutStats stats, const std::pair<time_t, dribas::workout>& entry) const
    {
      const auto& w = entry.second;
      if (w.distance > 0 && w.avgPaceMinPerKm > 0) {
        stats.count++;
        stats.max_distance = std::max(stats.max_distance, w.distance);
        stats.min_pace = std::min(stats.min_pace, w.avgPaceMinPerKm);
      }
      return stats;
    }
  };
}

dribas::RacePrediction dribas::predict_result(const std::map<time_t, workout>& workouts)
{
  RacePrediction prediction = {0.0, 0.0, 0.0, 0.0};
  WorkoutStats stats = std::accumulate(workouts.begin(), workouts.end(), WorkoutStats{}, WorkoutStats());

  if (stats.count == 0) {
    return prediction;
  }
  double max_distance = stats.max_distance;
  double best_pace = stats.min_pace;
  prediction.fiveKm = best_pace * 1.06;
  prediction.tenKm = best_pace * 1.08;
  if (max_distance >= 10.0) {
    prediction.halfMarathon = best_pace * 1.12;
  } else {
    prediction.halfMarathon = prediction.tenKm * 1.05;
  }
  if (max_distance >= 21.0) {
    prediction.marathon = best_pace * 1.18;
  } else if (max_distance >= 10.0) {
    prediction.marathon = prediction.halfMarathon * 1.10;
  } else {
    prediction.marathon = prediction.halfMarathon * 1.15;
  }
  return prediction;
}

std::ostream& dribas::operator<<(std::ostream& os, const RacePrediction& prediction)
{
  StreamGuard guard(os);
  os << "=== Race Time Predictions ===\n";
  os << "  5K:    " << std::fixed << std::setprecision(2) << prediction.fiveKm << " min/km\n";
  os << "  10K:   " << prediction.tenKm << " min/km\n";
  os << "  21.1K: " << prediction.halfMarathon << " min/km\n";
  os << "  42.2K: " << prediction.marathon << " min/km\n";
  return os;
}