#include "shape-utils.hpp"
#include <map>
#include <iterator>
#include <algorithm>
#include "rectangle.hpp"
#include "concave.hpp"
#include "complexquad.hpp"

savintsev::Shape * savintsev::createShape(std::istream & in, std::string name)
{
  std::string title;
  in >> title;
  if (name == "rectangle")
  {
    double n[4];
    for (size_t i = 0; i < 4; ++i)
    {
      in >> n[i];
    }
    return new Rectangle({n[0], n[1]}, {n[2], n[3]}, title);
  }
  if (name == "complexquad")
  {
    double n[8];
    for (size_t i = 0; i < 8; ++i)
    {
      in >> n[i];
    }
    return new Complexquad({n[0], n[1]}, {n[2], n[3]}, {n[4], n[5]}, {n[6], n[7]}, title);
  }
  if (name == "concave")
  {
    double n[8];
    for (size_t i = 0; i < 8; ++i)
    {
      in >> n[i];
    }
    return new Concave({n[0], n[1]}, {n[2], n[3]}, {n[4], n[5]}, {n[6], n[7]}, title);
  }
  return nullptr;
}
