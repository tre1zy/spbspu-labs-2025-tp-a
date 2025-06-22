#include "validator.hpp"
#include <fstream>
#include <unordered_map>

bool savintsev::has_savi_extension(const std::string & filename)
{
  if (filename.size() < 5)
  {
    return false;
  }
  return filename.substr(filename.size() - 5) == ".savi";
}

bool savintsev::validate_savi_file(const std::string & filename)
{
  if (!has_savi_extension(filename))
  {
    return false;
  }

  std::ifstream file(filename);
  if (!file)
  {
    return false;
  }

  std::unordered_map< std::string, int > shape_arg;
  shape_arg["rectangle"] = 4;
  shape_arg["circle"] = 3;
  shape_arg["triangle"] = 6;
  shape_arg["concave"] = 8;
  shape_arg["complexquad"] = 8;

  std::string shape, name;
  double value;

  while (file >> shape >> name)
  {
    auto it = shape_arg.find(shape);
    if (it == shape_arg.end())
    {
      return false;
    }

    int argCount = it->second;
    for (int i = 0; i < argCount; ++i)
    {
      if (!(file >> value))
      {
        return false;
      }
    }
  }

  return file.eof();
}
