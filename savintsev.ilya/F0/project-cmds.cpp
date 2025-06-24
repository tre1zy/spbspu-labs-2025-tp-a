#include "project-cmds.hpp"
#include <fstream>
#include <shape-utils.hpp>
#include "file-system.hpp"

void savintsev::open(std::istream & in, std::ostream & out, Projects & projs)
{
  std::string filename;
  in >> filename;

  read_savi_file(filename, projs);

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
      write_savi_file(filename, projs[filename]);
      answered = true;
    }
    else if (answer.substr(0, 1) == "N" || answer.substr(0, 1) == "n")
    {
      answered = true;
    }
  }

  projs.erase(filename);
  out << "The project \"" << filename << "\" was successfully closed\n";
}

void savintsev::create(std::istream & in, std::ostream & out, Projects & projs)
{
  std::string name;
  in >> name;
  projs[name];
  out << "The project \"" << name << "\" was successfully created\n";
}

void savintsev::copy(std::istream & in, std::ostream & out, Projects & projs)
{
  std::string proj, new_name;
  in >> proj >> new_name;
  projs[new_name] = projs.at(proj);
  out << "The project \"" << proj << "\" was successfully copied to \"" << new_name << "\"\n";
}

void savintsev::rename_project(std::istream & in, std::ostream & out, Projects & projs)
{
  std::string proj, new_name;
  in >> proj >> new_name;
  projs[new_name] = projs.at(proj);
  projs.erase(proj);
  out << "The project \"" << proj << "\" was successfully renamed to \"" << new_name << "\"\n";
}

void savintsev::save(std::istream & in, std::ostream & out, Projects & projs)
{
  std::string proj;
  in >> proj;
  write_savi_file(proj, projs.at(proj));
  out << "The project was successfully saved in " << proj << ".savi\n";
}

void savintsev::save_as(std::istream & in, std::ostream & out, Projects & projs)
{
  std::string proj, new_file;
  in >> proj >> new_file;
  write_savi_file(new_file, projs.at(proj));
  out << "The project was successfully saved in " << new_file << ".savi\n";
}

void savintsev::print(std::ostream & out, Projects & projs)
{
  out << "== List of open projects ==\n";
  size_t counter = projs.size();
  for (auto it = projs.begin(); it != projs.end(); ++it)
  {
    out << "[" << counter-- << "] " << it->first << '\n';
  }
}
