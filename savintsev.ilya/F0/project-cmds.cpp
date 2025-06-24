#include "project-cmds.hpp"
#include <fstream>
#include <shape-utils.hpp>
#include "file-system.hpp"

void savintsev::open(std::istream & in, std::ostream & out, Projects & projs)
{
  std::string filename;
  in >> filename;

  if (!validate_savi_file(filename))
  {
    throw std::runtime_error("ERROR: Can't open file " + get_filename_wext(filename) + "\n");
  }

  std::ifstream file(filename);

  Project project;

  while (!file.eof())
  {
    std::string figure;
    std::string name;
    file >> figure >> name;
    Layer new_pair;
    Shape * new_shape = createShape(file, figure);
    if (!new_shape)
    {
      throw std::runtime_error("UNKNOWN ERROR: Invalid file" + get_filename_wext(filename) + "\n");
    }
    new_pair = {name, new_shape};
    project.push_back(new_pair);
  }

  projs[get_filename(filename)] = project;
}

void savintsev::close(std::istream & in, std::ostream & out, Projects & projs)
{
  std::string filename;
  in >> filename;

  if (projs.erase(get_filename(filename)))
  {
    out << "The project was successfully closed\n";
    return;
  }
  throw std::out_of_range("ERROR");
}

void savintsev::create(std::istream & in, std::ostream & out, Projects & projs)
{
  std::string name;
  in >> name;

  projs[get_filename(name)];
}

void savintsev::save(std::istream & in, std::ostream & out, Projects & projs)
{
  std::string proj;
  in >> proj;

  std::ofstream file;

  file.open(proj + ".savi");

  
}
