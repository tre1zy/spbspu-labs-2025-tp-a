#ifndef VALIDATOR_HPP
#define VALIDATOR_HPP
#include <string>

namespace savintsev
{
  bool has_savi_extension(const std::string & filename);
  bool validate_savi_file(const std::string & filename);
}

#endif
