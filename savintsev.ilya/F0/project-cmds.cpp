#include "project-cmds.hpp"
#include <fstream>
#include <algorithm>
#include <functional>
#include <shape-utils.hpp>
#include "file-system.hpp"

void savintsev::open(std::istream & in, std::ostream & out, Projects & projs)
{
  std::string filename;
  in >> filename;

  read_savi_file(filename, projs);

  out << "Project \"" << get_filename(filename) << "\" was successfully opened\n";
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
  out << "Project \"" << filename << "\" was successfully closed\n";
}

void savintsev::create(std::istream & in, std::ostream & out, Projects & projs)
{
  std::string name;
  in >> name;
  projs[name];
  out << "Project \"" << name << "\" was successfully created\n";
}

void savintsev::copy(std::istream & in, std::ostream & out, Projects & projs)
{
  std::string proj, new_name;
  in >> proj >> new_name;
  projs[new_name] = projs.at(proj);
  out << "Project \"" << proj << "\" was successfully copied to \"" << new_name << "\"\n";
}

void savintsev::rename_project(std::istream & in, std::ostream & out, Projects & projs)
{
  std::string proj, new_name;
  in >> proj >> new_name;
  projs[new_name] = projs.at(proj);
  projs.erase(proj);
  out << "Project \"" << proj << "\" was successfully renamed to \"" << new_name << "\"\n";
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

namespace
{
  struct SaveProject
  {
    template< typename Iterator >
    void operator()(Iterator & entry) const
    {
      savintsev::write_savi_file(entry.first, entry.second);
    }
  };
}

void savintsev::save_all(std::ostream & out, Projects & projs)
{
  std::for_each(projs.begin(), projs.end(), SaveProject());
  out << "All projects were saved successfully\n";
}

void savintsev::merge(std::istream & in, std::ostream & out, Projects & projs)
{
  std::string proj, proj1, proj2;
  in >> proj >> proj1 >> proj2;
  if (proj == proj1)
  {
    projs.at(proj).insert(projs.at(proj).end(), projs.at(proj2).begin(), projs.at(proj2).end());
  }
  else if (proj == proj2)
  {
    projs.at(proj).insert(projs.at(proj).begin(), projs.at(proj1).begin(), projs.at(proj1).end());
  }
  else
  {
    projs[proj].insert(projs.at(proj).end(), projs.at(proj2).begin(), projs.at(proj2).end());
    projs[proj].insert(projs.at(proj).end(), projs.at(proj1).begin(), projs.at(proj1).end());
  }
  out << "Project \"" << proj1 << "\" has been successfully merged with project \"" << proj2 << "\"\n";
}

void savintsev::create_rectangle(std::istream & in, std::ostream & out, Projects & projs)
{
  std::string proj, name;
  short int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
  in >> proj >> name >> x1 >> y1 >> x2 >> y2;
  projs.
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
