#include <iostream>
#include <iomanip>
#include <algorithm>
#include <parser.hpp>
#include "processors.hpp"
#include "content_print.hpp"

int main(int argc, char** argv)
{
  using namespace rychkov::entities;
  Literal literals[] = {
        {"abc", "u16", Literal::String},
        {"\\n", "u16", Literal::Char},
        {"0x182", "ull", Literal::Number},
        {"true", {}, Literal::Logic}
      };
  rychkov::ContentPrinter printer{std::cout};
  for (const Literal& i: literals)
  {
    printer(i);
  }
  std::cout << '\n';

  using namespace rychkov::typing;
  Type types[] = {
        {"int", Type::Int, nullptr, false, false, true, false, Type::SHORT},
        {{}, Type::Combination, types + 0, true, true},
        {{}, Type::Array, types + 1, true, true, false, false, Type::NO_LENGTH, true, 2},
        {{}, Type::Function, types + 1, false, false, false, false, Type::NO_LENGTH, false, 0, {types[0], types[2]}},
        {{}, Type::Pointer, types + 3, true, true},
        {{}, Type::Pointer, types + 3, true},
        {{}, Type::Pointer, types + 5},
        {{}, Type::Array, types + 6, false, false, false, false, Type::NO_LENGTH, true, 2},
        {{}, Type::Array, types + 7, false, false, false, false, Type::NO_LENGTH},
        {{}, Type::Pointer, types + 8, false, true},
      };
  for (const Type& i: types)
  {
    printer(Declaration{Variable{i, "var"}});
  }
  printer(Declaration{Function{types[3], "foo", {"arg1", "arg2"}}});
  printer(Declaration{Struct{"S", {{types[0], "field1"}, {types[1], "field2"}}, {{}, {}}}});
  printer(Declaration{Enum{"E", {{"val1", 0}, {"val2", 1}}}});
  printer(Declaration{Union{"U", {{types[0], "field1"}, {types[1], "field2"}}}});
  printer(Declaration{Alias{types[0], "bool"}});
  printer(Declaration{Function{types[3], "foo", {"arg1", "arg2"}, true, {}}});
  /*std::cout << std::fixed << std::setprecision(1);
  using processor = rychkov::MainProcessor;
  using parser_type = rychkov::Parser< processor >;
  parser_type::map_type call_map = {
    {"help", &processor::help}
  };
  parser_type parser{{std::cin, std::cout, std::cerr}, {}, std::move(call_map)};
  if (!parser.processor.init(parser.context, argc, argv))
  {
    return 1;
  }

  while (parser.run())
  {}*/
}
