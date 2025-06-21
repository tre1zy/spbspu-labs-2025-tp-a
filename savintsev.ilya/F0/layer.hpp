#ifndef LAYER_HPP
#define LAYER_HPP
#include <string>
#include <list>
#include <shape.hpp>

namespace savintsev
{
  class Layer
  {
  public:

  private:
    std::string name = "";
    Shape * shape;
  };
}

#endif
