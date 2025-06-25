#include <map>
#include <string>
#include <functional>
#include <iostream>
#include "cmd_line_args.hpp"
#include "commands.hpp"

namespace
{
  int checkArguments(const int argc)
  {
    if (argc > 1)
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }
}

int main(const int argc, const char * const * argv)
{
  using namespace petrov;
  if (checkArguments(argc))
  {
    processCmdLineAgruments(argc, argv);
  }
  std::map< std::string, std::function< void() > > cmds;
  cmds["create_map"] = std::bind(createMap);
}
