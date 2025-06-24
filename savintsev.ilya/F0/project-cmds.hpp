#ifndef PROJECT_CMDS_HPP
#define PROJECT_CMDS_HPP
#include <iostream>
#include "project-body.hpp"

namespace savintsev
{
  void open(std::istream & in, std::ostream & out, Projects & projs);
  void close(std::istream & in, std::ostream & out, Projects & projs);
  void create(std::istream & in, std::ostream & out, Projects & projs);
  void copy(std::istream & in, std::ostream & out, Projects & projs);
  void rename(std::istream & in, std::ostream & out, Projects & projs);
  void save(std::istream & in, std::ostream & out, Projects & projs);
  void save_as(std::istream & in, std::ostream & out, Projects & proj);
  void print(std::ostream & out, Projects & projs);
}

#endif
