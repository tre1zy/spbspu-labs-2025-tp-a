#include "command_handler.hpp"
#include <limits>
#include "commands.hpp"

void puzikov::CommandHandler::readCommands(std::istream &in, std::ostream &out)
{
  while (!std::cin.eof())
  {
    std::cin >> cmd;
    try
    {
      commands.at(cmd)(in, out);
    }
    catch (...)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}

puzikov::CommandHandler::CommandHandler(std::vector< Polygon > &refPolys):
  polys(refPolys)
{
  commands = {{"AREA", std::bind(puzikov::areaCommand, _1, _2, std::ref(polys))},
              {"MAX", std::bind(puzikov::maxCommand, _1, _2, std::ref(polys))},
              {"MIN", std::bind(puzikov::minCommand, _1, _2, std::ref(polys))},
              {"COUNT", std::bind(puzikov::countCommand, _1, _2, std::ref(polys))},
              {"RMECHO", std::bind(puzikov::rmEchoCommand, _1, _2, std::ref(polys))},
              {"SAME", std::bind(puzikov::sameCommand, _1, _2, std::ref(polys))}};
};
