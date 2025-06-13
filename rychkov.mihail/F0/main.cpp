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
int b = 0x192'023ULL ;
int main()
{
  //
}
)code";

int main(int argc, char** argv)
{
  {
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
    std::cout << "\n##########\n";
    cparser.print(std::cout);
  }
}
