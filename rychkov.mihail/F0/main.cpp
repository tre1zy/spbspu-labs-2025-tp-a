#include <iostream>
#include <sstream>
#include "processors.hpp"
#include "code_parsers.hpp"
#include "content_print.hpp"

static const char code[] = R"code(
// #include <ios> \
#define def number
// #define macro(num, suf) #num##suf
/* #command
// # define a*/

struct A
{;};
A a = {};
int b = 0x192'023ULL;
)code";

int main(int argc, char** argv)
{
  {
    rychkov::CParseContext context{std::cerr, "main.c"};
    rychkov::TypeParser tparser;
    tparser.append(context, "const");
    tparser.append(context, rychkov::typing::Type{"int", rychkov::typing::Type::Int});
    tparser.append(context, '*');
    tparser.append(context, "const");
    tparser.append(context, '(');
    tparser.append(context, '*');
    tparser.append(context, '(');
    tparser.append(context, "var");
    tparser.append(context, ')');
    tparser.append(context, '(');
    tparser.append(context, rychkov::typing::Type{"char", rychkov::typing::Type::Int});
    tparser.append(context, "arg1");
    tparser.append(context, ',');
    tparser.append(context, "const");
    tparser.append(context, rychkov::typing::Type{"int", rychkov::typing::Type::Int});
    tparser.append(context, '(');
    tparser.append(context, '*');
    tparser.append(context, ')');
    tparser.append(context, '[');
    tparser.append(context, ']');
    tparser.append(context, ')');
    tparser.append(context, ')');
    tparser.append(context, '[');
    tparser.append(context, 3ULL);
    tparser.append(context, ']');
    tparser.append(context, '[');
    tparser.append(context, 4ULL);
    tparser.append(context, ']');
    if (tparser.is_function())
    {
      rychkov::operator<<(std::cout << "func: ", tparser.function()) << '\n';
    }
    else
    {
      rychkov::operator<<(std::cout << "var: ", tparser.variable()) << '\n';
    }
    std::cout << std::boolalpha << tparser.ready() << '\n';
  }
  /*{
    rychkov::CParseContext context{std::cerr, "main.c"};
    std::istringstream in(code);
    rychkov::Preprocessor preproc{};
    while (in)
    {
      std::string line;
      std::getline(in, line);
      preproc.parse(context, std::move(line));
    }
  }
  std::cout << '\n';
  {
    rychkov::CParseContext context{std::cerr, "main.c"};
    std::istringstream in(code);
    rychkov::Lexer lexer{};
    rychkov::Preprocessor preproc{lexer};
    while (in)
    {
      std::string line;
      std::getline(in, line);
      preproc.parse(context, std::move(line));
    }
    lexer.flush(context);
  }
  std::cout << '\n';
  {
    rychkov::CParseContext context{std::cerr, "main.c"};
    std::istringstream in(code);
    rychkov::CParser cparser;
    rychkov::Lexer lexer{cparser};
    rychkov::Preprocessor preproc{lexer};
    while (in)
    {
      std::string line;
      std::getline(in, line);
      preproc.parse(context, std::move(line));
    }
    lexer.flush(context);
    cparser.print(std::cout);
  }*/
}
