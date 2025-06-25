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
  struct ShapeNameEquals
  {
    const std::string & name;
    ShapeNameEquals(const std::string & n):
      name(n)
    {}
    bool operator()(const savintsev::Layer & layer) const
    {
      return layer.second && layer.second->get_name() == name;
    }
  };
  std::string make_sh(std::istream & in, savintsev::Projects & projs, std::string shape_name)
  {
    std::string proj;
    in >> proj;
    projs.at(proj);
    savintsev::Shape * shape = savintsev::createShape(in, shape_name);
    if (!shape)
    {
      throw std::runtime_error("Incorrect description of the shape");
    }
    projs[proj].push_front({shape_name, shape});
    return proj;
  }
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
  std::string name = make_sh(in, projs, "rectangle");
  out << "The rectangle was successfully created and inserted into \"" << name << "\"\n";
}

void savintsev::create_complexquad(std::istream & in, std::ostream & out, Projects & projs)
{
  std::string name = make_sh(in, projs, "complexquad");
  out << "The complexquad was successfully created and inserted into \"" << name << "\"\n";
}

void savintsev::delete_shape(std::istream & in, std::ostream & out, Projects & projs)
{
  std::string proj, shape_name;
  in >> proj >> shape_name;

  Project & pr = projs.at(proj);

  auto it = std::find_if(pr.begin(), pr.end(), ShapeNameEquals(shape_name));
  if (it != pr.end())
  {
    delete it->second;
    pr.erase(it);
    out << "\"" << shape_name << "\" was successfully removed from \"" << proj << "\"\n";
    return;
  }

  out << "\"" << proj << "\" does not have a \"" << shape_name << "\"\n";
}

void savintsev::create_concave(std::istream & in, std::ostream & out, Projects & projs)
{
  std::string name = make_sh(in, projs, "concave");
  out << "The concave was successfully created and inserted into \"" << name << "\"\n";
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

void savintsev::make_back()
{
  throw std::bad_alloc();
}
