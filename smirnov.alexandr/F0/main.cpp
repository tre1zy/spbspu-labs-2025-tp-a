#include <functional>
#include <limits>
#include <guard.hpp>
#include <delimetr.hpp>
#include "commands.hpp"
#include "dictionary.hpp"

int main()
{
  using namespace smirnov;
  Dicts dicts;
  std::unordered_map< std::string, std::function< void(std::istream &) > > commands;
  using namespace std::placeholders;
  commands["create"] = std::bind(createCommand, std::ref(dicts), _1, std::ref(std::cout));
  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      commands.at(cmd)(std::cin);
    }
    catch (const std::out_of_range &)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
    catch (const std::exception &)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
}
