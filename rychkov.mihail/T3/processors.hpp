#ifndef PROCESSORS_HPP
#define PROCESSORS_HPP

#include "parser.hpp"

namespace rychkov
{
  class MainProcessor
  {
  public:
    bool init(ParserContext& context, int argc, char** argv);
  };
}

#endif
