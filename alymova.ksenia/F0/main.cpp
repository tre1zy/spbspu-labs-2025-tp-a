#include <iostream>
#include <fstream>
#include <clocale>
#include <cstring>
#include "commands.hpp"

int main(int argc, char** argv)
{
  using namespace alymova;
  std::setlocale(LC_CTYPE, "rus");

  if (argc > 2)
  {
    std::cerr << "<INCORRECT ARGUMENTS>\n";
    return 1;
  }
  std::istream* input = &std::cin;
  std::ifstream file;
  if (argc == 2)
  {
    if (std::strcmp(argv[1], "--help") == 0)
    {
      return 0;
    }
    file.open(argv[1]);
    if (!file.is_open())
    {
      std::cerr << "<INCORRECT FILE>\n";
      return 1;
    }
    input = &file;
  }

  try
  {
    TranslateSet tr1{"yabloko"};
    TranslateSet tr2{"banan", "bananovyj"};
    Dictionary dict{{"apple", tr1}, {"banana", tr2}};
    Dictionary dict1(dict);
    DictSet set{{"first", dict}, {"second", dict}};
    translate(*input, std::cout, set);
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
}
