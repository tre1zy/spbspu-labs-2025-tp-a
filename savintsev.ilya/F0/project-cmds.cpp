#include "project-cmds.hpp"
#include <fstream>
#include <algorithm>
#include <functional>
#include <cmath>
#include <random>
#include <boost/gil.hpp>
#include <boost/gil/extension/io/bmp.hpp>
#include <shape-utils.hpp>
#include <scope-guard.hpp>
#include "file-system.hpp"
#include "confirmation-src.hpp"

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

  ConfirmationPrompt prompt(in, out);
  if (prompt.ask("Save project before closing?"))
  {
    write_savi_file(filename, projs.at(filename));
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
    projs[proj].push_back({shape_name, shape});
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

void savintsev::create_concave(std::istream & in, std::ostream & out, Projects & projs)
{
  std::string name = make_sh(in, projs, "concave");
  out << "The concave was successfully created and inserted into \"" << name << "\"\n";
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

void savintsev::copy_proj_or_shape(std::istream & in, std::ostream & out, Projects & projs)
{
  std::string proj, second, third;
  in >> proj >> second;

  if (in.peek() == '\n' || !(in >> third))
  {
    const Project & src_proj = projs.at(proj);
    Project new_proj;

    struct CloneInserter
    {
      Project & new_proj;
      void operator()(const std::pair< const std::string, Shape * > & layer) const
      {
        Shape * cloned_shape = layer.second->clone();
        cloned_shape->set_name(layer.second->get_name());
        new_proj.insert(new_proj.end(), std::make_pair(layer.first, cloned_shape));
      }
    };

    std::for_each(src_proj.begin(), src_proj.end(), CloneInserter{new_proj});

    projs[second] = std::move(new_proj);

    out << "Project \"" << proj << "\" was successfully copied to \"" << second << "\"\n";
  }
  else
  {
    Project & pr = projs.at(proj);

    auto it = std::find_if(pr.begin(), pr.end(), ShapeNameEquals(second));
    if (it == pr.end())
    {
      out << "\"" << second << "\" not found in project \"" << proj << "\"\n";
      return;
    }

    Shape * cloned = it->second->clone();
    cloned->set_name(third);

    pr.insert(std::next(it), std::make_pair(it->first, cloned));
    out << "\"" << second << "\" was copied as \"" << third << "\" in project \"" << proj << "\"\n";
  }
}

void savintsev::insert_shape(std::istream & in, std::ostream & out, Projects & projs)
{
  std::string src_proj, shape_name, dest_proj;
  in >> src_proj >> shape_name >> dest_proj;

  Project & from = projs.at(src_proj);
  Project & to = projs.at(dest_proj);

  auto it = std::find_if(from.begin(), from.end(), ShapeNameEquals(shape_name));
  if (it == from.end())
  {
    out << "\"" << shape_name << "\" not found in project \"" << src_proj << "\"\n";
    return;
  }

  Shape * cloned = it->second->clone();
  to.push_back(std::make_pair(it->first, cloned));
  cloned->set_name(shape_name + "_copy");

  out << "\"" << shape_name << "\" from \"" << src_proj << "\" was inserted into \"" << dest_proj << "\"\n";
}

void savintsev::rename_proj_or_shape(std::istream & in, std::ostream & out, Projects & projs)
{
  std::string proj, second, third;
  in >> proj >> second;

  if (in.peek() == '\n' || !(in >> third))
  {
    const std::string & new_name = second;
    projs[new_name] = projs.at(proj);
    projs.erase(proj);
    out << "Project \"" << proj << "\" was successfully renamed to \"" << new_name << "\"\n";
  }
  else
  {
    const std::string & old_name = second;
    const std::string & new_name = third;

    Project & pr = projs.at(proj);
    auto it = std::find_if(pr.begin(), pr.end(), ShapeNameEquals(old_name));

    if (it == pr.end())
    {
      out << "\"" << old_name << "\" not found in project \"" << proj << "\"\n";
      return;
    }

    it->first = new_name;
    it->second->set_name(new_name);
    out << "\"" << old_name << "\" was renamed to \"" << new_name << "\" in project \"" << proj << "\"\n";
  }
}

void savintsev::sf(std::istream & in, std::ostream & out, Projects & projs)
{
  std::string proj, name;
  int n;
  in >> proj >> name >> n;

  Project & pr = projs.at(proj);
  auto it = std::find_if(pr.begin(), pr.end(), ShapeNameEquals(name));

  if (it == pr.end())
  {
    out << "\"" << name << "\" not found in project \"" << proj << "\"\n";
    return;
  }

  Layer layer = *it;
  it = pr.erase(it);

  if (n > 0)
  {
    auto dest = it;
    while (n-- > 0 && dest != pr.end())
    {
      ++dest;
    }
    pr.insert(dest, std::move(layer));
  }
  else
  {
    auto dest = it;
    while (n++ < 0 && dest != pr.begin())
    {
      --dest;
    }
    pr.insert(dest, std::move(layer));
  }

  out << "\"" << name << "\" was moved in project \"" << proj << "\"\n";
}

void savintsev::sf_above(std::istream & in, std::ostream & out, Projects & projs)
{
  std::string proj, name;
  in >> proj >> name;

  Project & pr = projs.at(proj);
  auto it = std::find_if(pr.begin(), pr.end(), ShapeNameEquals(name));
  if (it == pr.end())
  {
    out << "\"" << name << "\" not found in project \"" << proj << "\"\n";
    return;
  }

  pr.splice(pr.end(), pr, it);
  out << "\"" << name << "\" was moved to the top in project \"" << proj << "\"\n";
}

void savintsev::sf_below(std::istream & in, std::ostream & out, Projects & projs)
{
  std::string proj, name;
  in >> proj >> name;

  Project & pr = projs.at(proj);
  auto it = std::find_if(pr.begin(), pr.end(), ShapeNameEquals(name));
  if (it == pr.end())
  {
    out << "\"" << name << "\" not found in project \"" << proj << "\"\n";
    return;
  }

  pr.splice(pr.begin(), pr, it);
  out << "\"" << name << "\" was moved to the bottom in project \"" << proj << "\"\n";
}

void savintsev::swap_shapes(std::istream & in, std::ostream & out, Projects & projs)
{
  std::string proj, name1, name2;
  in >> proj >> name1 >> name2;

  Project & pr = projs.at(proj);

  auto it1 = std::find_if(pr.begin(), pr.end(), ShapeNameEquals(name1));
  auto it2 = std::find_if(pr.begin(), pr.end(), ShapeNameEquals(name2));

  if (it1 == pr.end() || it2 == pr.end())
  {
    out << "One or both shapes not found in project \"" << proj << "\"\n";
    return;
  }

  if (it1 == it2)
  {
    out << "Cannot swap the same shape.\n";
    return;
  }

  std::iter_swap(it1, it2);
  out << "\"" << name1 << "\" and \"" << name2 << "\" were swapped in project \"" << proj << "\"\n";
}

void savintsev::move_abs(std::istream & in, std::ostream & out, Projects & projs)
{
  std::string proj, name;
  double x, y;
  in >> proj >> name >> x >> y;
  auto & pr = projs.at(proj);
  auto it = std::find_if(pr.begin(), pr.end(), ShapeNameEquals(name));
  if (it == pr.end())
  {
    out << "\"" << name << "\" not found in project \"" << proj << "\"\n";
    return;
  }
  it->second->move({x, y});
  out << "\"" << name << "\" was succesfully moved\n";
}

void savintsev::move_rel(std::istream & in, std::ostream & out, Projects & projs)
{
  std::string proj, name;
  double dx, dy;
  in >> proj >> name >> dx >> dy;
  auto & pr = projs.at(proj);
  auto it = std::find_if(pr.begin(), pr.end(), ShapeNameEquals(name));
  if (it == pr.end())
  {
    out << "\"" << name << "\" not found in project \"" << proj << "\"\n";
    return;
  }
  it->second->move(dx, dy);
  out << "\"" << name << "\" was succesfully moved\n";
}

void savintsev::move_all(std::istream & in, std::ostream & out, Projects & projs)
{
  std::string proj;
  double dx, dy;
  in >> proj >> dx >> dy;
  auto & pr = projs.at(proj);
  for (auto & layer : pr)
  {
    layer.second->move(dx, dy);
  }
  out << "All shapes moved by (" << dx << ", " << dy << ")\n";
}

void savintsev::scale_shape(std::istream & in, std::ostream & out, Projects & projs)
{
  std::string proj, name;
  double k;
  in >> proj >> name >> k;
  auto & pr = projs.at(proj);
  auto it = std::find_if(pr.begin(), pr.end(), ShapeNameEquals(name));
  if (it == pr.end())
  {
    out << "\"" << name << "\" not found in project \"" << proj << "\"\n";
    return;
  }

  it->second->scale(k);
  out << "\"" << name << "\" was succesfully scaled\n";
}

void savintsev::array_shapes(std::istream & in, std::ostream & out, Projects & projs)
{
  std::string proj, name;
  int n;
  double dx, dy;
  in >> proj >> name >> n >> dx >> dy;
  auto & pr = projs.at(proj);
  auto it = std::find_if(pr.begin(), pr.end(), ShapeNameEquals(name));
  if (it == pr.end())
  {
    out << "\"" << name << "\" not found in project \"" << proj << "\"\n";
    return;
  }
  Shape * base = it->second;
  for (int i = 1; i <= n; ++i)
  {
    Shape * copy = base->clone();
    std::string new_name = name + "_" + std::to_string(i);
    copy->set_name(new_name);
    copy->move(dx * i, dy * i);
    pr.insert(std::next(it), {new_name, copy});
  }
  out << n << " copies succesfully created\n";
}

void savintsev::reverse_project(std::istream & in, std::ostream & out, Projects & projs)
{
  std::string proj;
  in >> proj;
  auto & pr = projs.at(proj);
  pr.reverse();
  out << "Layers of \"" << proj << "\" reversed\n";
}

void savintsev::clear_project(std::istream & in, std::ostream & out, Projects & projs)
{
  std::string proj;
  in >> proj;
  auto & pr = projs.at(proj);

  ConfirmationPrompt prompt(in, out);
  if (prompt.ask("Are you sure you want to clear the project?"))
  {
    for (auto & l : pr)
    {
      delete l.second;
    }
    pr.clear();
    out << "Project \"" << proj << "\" was successfully cleared\n";
    return;
  }

  out << "Clear cancelled\n";
}

void savintsev::print(std::ostream & out, Projects & projs)
{
  out << "=== List of open projects ===\n";
  size_t counter = 1;
  for (auto it = projs.begin(); it != projs.end(); ++it)
  {
    out << "[" << counter++ << "] " << it->first << '\n';
  }
}

void savintsev::print_shapes(std::istream & in, std::ostream & out, Projects & projs)
{
  std::string proj;
  in >> proj;

  Project & pr = projs.at(proj);
  size_t counter = 1;
  out << "=== Layers from the top ===\n";
  out << "    Shape  Name\n";
  for (auto it = pr.begin(); it != pr.end(); ++it)
  {
    out << "[" << counter++ << "] ";
    if (it->first == "rectangle")
    {
      out << "Rectan ";
    }
    else if (it->first == "complexquad")
    {
      out << "Cxquad ";
    }
    else if (it->first == "concave")
    {
      out << "Concav ";
    }
    out << it->second->get_name() << '\n';
  }
}

void savintsev::print_info_about_shape(std::istream & in, std::ostream & out, Projects  & projs)
{
  std::string proj, name;
  in >> proj >> name;

  Project & pr = projs.at(proj);
  auto it = std::find_if(pr.begin(), pr.end(), ShapeNameEquals(name));
  if (it == pr.end())
  {
    out << "Figure \"" << name << "\" not found in project \"" << proj << "\"\n";
    return;
  }
  ScopeGuard scope(in);
  in.precision(2);
  out << "=== Info about shape ===\n";
  out << "Type " << it->first << '\n';
  out << "Name " << it->second->get_name() << '\n';
  out << "Layr " << std::distance(pr.begin(), it) + 1 << '\n';
  const rectangle_t & frame = it->second->get_frame_rect();
  out << "Cent (x: " << frame.pos.x << ", y: " << frame.pos.y << ")\n";
  out << "Size (W: " << frame.width << ", H: " << frame.height << ")\n";
  out << "=== Coordinates of points ===\n";
  point_t ps[4];
  size_t counter = it->second->get_all_points(ps);
  for (size_t i = 1; i < counter + 1; ++i)
  {
    out << "[" << i << "] " << "(x: " << ps[i].x << ", y: " << ps[i].y << ")\n";
  }
}

namespace gil = boost::gil;

namespace
{
  bool point_in_polygon(const savintsev::point_t * pts, size_t count, double x, double y)
  {
    bool inside = false;
    for (size_t i = 0, j = count - 1; i < count; j = i++)
    {
      double xi = pts[i].x, yi = pts[i].y;
      double xj = pts[j].x, yj = pts[j].y;

      bool intersect = ((yi > y) != (yj > y)) && (x < (xj - xi) * (y - yi) / (yj - yi + 1e-15) + xi);
      if (intersect)
      {
        inside = !inside;
      }
    }
    return inside;
  }

  void fill_shape_simple(
    gil::rgb8_view_t & view,
    const savintsev::point_t* pts,
    size_t count,
    gil::rgb8_pixel_t color)
  {
    if (count < 3)
    {
      return;
    }

    int width = view.width();
    int height = view.height();


    for (int y = 0; y < height; ++y)
    {
      for (int x = 0; x < width; ++x)
      {
        double fx = x - width / 2.0;
        double fy = height / 2.0 - y;

        if (point_in_polygon(pts, count, fx + 0.5, fy + 0.5))
        {
          view(x, y) = color;
        }
      }
    }
  }
}

void savintsev::render(std::istream& in, std::ostream& out, Projects& projs)
{
  std::string proj_name, image_name;
  int width, height;
  in >> proj_name >> image_name >> width >> height;

  if (projs.find(proj_name) == projs.end())
  {
    out << "Project \"" << proj_name << "\" not found\n";
    return;
  }

  auto & proj = projs.at(proj_name);

  gil::rgb8_image_t image(width, height);
  auto view = gil::view(image);

  gil::fill_pixels(view, gil::rgb8_pixel_t(255, 255, 255));

  std::mt19937 rng(std::random_device{}());
  std::uniform_int_distribution< int > dist(50, 255);

  for (const auto & layer : proj)
  {
    savintsev::point_t pts[4];
    size_t count = layer.second->get_all_points(pts);

    gil::rgb8_pixel_t color
    (
      static_cast< uint8_t >(dist(rng)),
      static_cast< uint8_t >(dist(rng)),
      static_cast< uint8_t >(dist(rng))
    );

    fill_shape_simple(view, pts, count, color);
  }

  gil::write_view(image_name + ".bmp", view, gil::bmp_tag{});

  out << "Project \"" << proj_name << "\" rendered successfully to \"" << image_name << ".bmp\"\n";
}
