#ifndef FILE_SYSTEM_HPP
#define FILE_SYSTEM_HPP
#include <string>
#include "project-body.hpp"

namespace savintsev
{
  std::string get_filename(const std::string & filename);
  std::string get_filename_wext(const std::string & filename);

  bool has_savi_extension(const std::string & filename);
  bool validate_savi_file(const std::string & filename);

  void write_file_data(std::ostream & out, Project & proj);
}

#endif
