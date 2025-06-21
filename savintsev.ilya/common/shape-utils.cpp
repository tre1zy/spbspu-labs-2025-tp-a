#include "shape-utils.hpp"
#include <cstring>
#include "rectangle.hpp"
#include "concave.hpp"
#include "complexquad.hpp"

savintsev::Shape * savintsev::createShape(char * name, const double * n)
{
  if (!std::strcmp(name, "rectangle"))
  {
    return new Rectangle({n[0], n[1]}, {n[2], n[3]});
  }
  if (!std::strcmp(name, "complexquad"))
  {
    return new Complexquad({n[0], n[1]}, {n[2], n[3]}, {n[4], n[5]}, {n[6], n[7]});
  }
  if (!std::strcmp(name, "concave"))
  {
    return new Concave({n[0], n[1]}, {n[2], n[3]}, {n[4], n[5]}, {n[6], n[7]});
  }
  return nullptr;
}
