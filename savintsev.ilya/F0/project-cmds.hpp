#ifndef PROJECT_CMDS_HPP
#define PROJECT_CMDS_HPP
#include <iostream>
#include "project-body.hpp"

namespace savintsev
{
  void open(std::istream & in, std::ostream & out, Projects & proj);
  void close(std::istream & in, std::ostream & out, Projects & proj);
  void create(std::istream & in, std::ostream & out, Projects & proj);
  void copy(std::istream & in, std::ostream & out, Projects & proj);
  void rename(std::istream & in, std::ostream & out, Projects & proj);
  void save(std::istream & in, std::ostream & out, Projects & proj);
  void save_as(std::istream & in, std::ostream & out, Projects & proj);
}

#endif
