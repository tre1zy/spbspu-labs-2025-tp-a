#ifndef CMDS_HPP
#define CMDS_HPP

#include <map>
#include "workout.hpp"

namespace dribas
{
  using suite = std::map< size_t, std::map< time_t, dribas::workout > >;
  void add_training_manual(std::istream&, std::ostream&, suite&);
  void add_training_from_file(std::istream&, std::ostream&, suite&);//2
  void show_all_trainings(std::ostream&, const suite&);//2
  void show_trainings_by_date(std::istream&, std::ostream&, const suite&);//2
  void query_trainings(std::istream&, std::ostream&, suite&);//2;
  void calculate_average(std::istream&, std::ostream&, suite&);//2;
  void get_top_trainings(std::istream&, std::ostream&, const suite&);//3
  void get_under_trainings(std::istream&, std::ostream&, const suite&);
  void survival_score(std::ostream&, const suite&);//2;
  void show_rest(std::istream&, std::ostream&, const suite&);//2
  void predict_result(std::ostream&, const suite&);
}



#endif
