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
    out << "ERROR: Can't open file " + get_filename_wext(filename) + "\n";
    throw std::runtime_error("");
  }

  std::ifstream file(filename);

  Project project;

  while (!file.eof())
  {
    std::string figure;
    file >> figure;
    Layer new_pair;
    Shape * new_shape = createShape(file, figure);
    if (!new_shape)
    {
      out << "UNKNOWN ERROR: Invalid file" + get_filename_wext(filename) + "\n";
      throw std::runtime_error("");
    }
    new_pair = {figure, new_shape};
    project.push_back(new_pair);
  }

  projs[get_filename(filename)] = project;

  out << "The project \"" << get_filename(filename) << "\" was successfully opened\n";
}

void savintsev::close(std::istream & in, std::ostream & out, Projects & projs)
{
  std::string filename;
  in >> filename;

  projs.at(filename);

  bool answered = false;
  while (!answered)
  {
    out << "Save project before closing? [Y / N]\n";
    std::string answer;
    in >> answer;
    if (answer.substr(0, 1) == "Y" || answer.substr(0, 1) == "y")
    {
      //save_project(projs[filename]);
      answered = true;
    }
    else if (answer.substr(0, 1) == "N" || answer.substr(0, 1) == "n")
    {
      answered = true;
    }
  }

  projs.erase(filename);
  out << "The project was successfully closed\n";
}

void savintsev::create(std::istream & in, std::ostream & out, Projects & projs)
{
  std::string name;
  in >> name;

  projs[name];

  out << "The project \"" << name << "\" was successfully created\n";
}

void savintsev::save(std::istream & in, std::ostream & out, Projects & projs)
{
  std::string proj;
  in >> proj;

  projs.at(proj);

  out << "Saving file...\n";

  std::ofstream file;

  file.open(proj + ".savi");

  if (!file)
  {
    out << "An unexpected error occurred\n";
    return;
  }

  write_file_data(file, projs[proj]);

  file.close();

  out << "The project was successfully saved in " << proj << ".savi\n";
}

void savintsev::save_as(std::istream & in, std::ostream & out, Projects & projs)
{
  std::string proj, new_file;
  in >> proj;

  projs.at(proj);

  in >> new_file;

  out << "Saving " << proj << " as " << new_file << ".savi\n";

  std::ofstream file;

  file.open(new_file + ".savi");

  if (!file)
  {
    out << "An unexpected error occurred\n";
    return;
  }

  write_file_data(file, projs[proj]);

  file.close();

  out << "The project was successfully saved in " << new_file << ".savi\n";
}

void savintsev::print(std::ostream & out, Projects & projs)
{
  for (auto it = projs.begin(); it != projs.end(); ++it)
  {
    out << "Project " << it->first << '\n';
    for (auto jt = it->second.begin(); jt != it->second.end(); ++jt)
    {
      out << jt->first << '\n';
    }
  }
}
