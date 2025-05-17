#include <iostream>
#include <map>
#include "parser.hpp"
#include "processors.hpp"

int main(int argc, char** argv)
{
  using processor = rychkov::MainProcessor;
  using parser_type = rychkov::Parser< processor >;
  parser_type::map_type call_map = {};
  parser_type parser{{std::cin, std::cout, std::cerr}, {}, std::move(call_map)};
  if (!parser.processor.init(parser.context, argc, argv))
  {
    return 1;
  }

  while (parser.run())
  {}
}
