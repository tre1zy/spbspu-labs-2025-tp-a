#include "file-system.hpp"
#include <fstream>
#include <unordered_map>
#include "shape-utils.hpp"

std::string savintsev::get_filename(const std::string & filename)
{
  size_t slash = filename.rfind('/');
  size_t dot = filename.rfind('.');
  if (slash == std::string::npos || dot == std::string::npos)
  {
    return filename;
  }
  return filename.substr(slash + 1, dot - slash - 1);
}

std::string savintsev::get_filename_wext(const std::string & filename)
{
  size_t slash = filename.rfind('/');
  if (slash == std::string::npos)
  {
    return filename;
  }
  return filename.substr(slash + 1);
}

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

void savintsev::read_savi_file(const std::string & filename, Projects & projs)
{
  std::ifstream file(filename);

  if (!file || !validate_savi_file(filename))
  {
    throw std::runtime_error("Can't open file " + get_filename_wext(filename));
  }

  Project project;

  while (!file.eof())
  {
    std::string figure;
    file >> figure;
    Layer new_pair;
    Shape * new_shape = createShape(file, figure);
    if (!new_shape)
    {
      throw std::runtime_error("Invalid file " + get_filename_wext(filename));
    }
    new_pair = {figure, new_shape};
    project.push_back(new_pair);
  }

  projs[get_filename(filename)] = project;
}

void savintsev::write_savi_file(const std::string & filename, Project & proj)
{
  std::ofstream file(filename + ".savi");

  if (!file)
  {
    throw std::runtime_error("Failed to save project to " + filename + ".savi");
  }

  for (auto it = proj.begin(); it != proj.end(); ++it)
  {
    file << it->first << " " << it->second->get_name();
    point_t ps[4];
    for (size_t i = 0; i < it->second->get_all_points(ps); ++i)
    {
      file << " " << ps[i].x << " " << ps[i].y;
    }
    file << '\n';
  }
}
