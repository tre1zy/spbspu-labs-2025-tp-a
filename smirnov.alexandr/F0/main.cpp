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
  commands["add"] = std::bind(addCommand, std::ref(dicts), _1, std::ref(std::cout));
  commands["translate"] = std::bind(translateCommand, std::ref(dicts), _1, std::ref(std::cout));
  commands["remove"] = std::bind(removeCommand, std::ref(dicts), _1, std::ref(std::cout));
  commands["print"] = std::bind(printCommand, std::ref(dicts), _1, std::ref(std::cout));
  commands["save"] = std::bind(saveCommand, std::ref(dicts), _1, std::ref(std::cout));
  commands["merge"] = std::bind(mergeCommand, std::ref(dicts), _1, std::ref(std::cout));
  commands["delete"] = std::bind(deleteCommand, std::ref(dicts), _1, std::ref(std::cout));
  commands["edit"] = std::bind(editCommand, std::ref(dicts), _1, std::ref(std::cout));
  commands["rename"] = std::bind(renameCommand, std::ref(dicts), _1, std::ref(std::cout));
  commands["move"] = std::bind(moveCommand, std::ref(dicts), _1, std::ref(std::cout));
  commands["copy"] = std::bind(copyCommand, std::ref(dicts), _1, std::ref(std::cout));
  commands["intersect"] = std::bind(intersectCommand, std::ref(dicts), _1, std::ref(std::cout));
  commands["difference"] = std::bind(differenceCommand, std::ref(dicts), _1, std::ref(std::cout));
  commands["unique"] = std::bind(uniqueCommand, std::ref(dicts), _1, std::ref(std::cout));
  commands["prefix"] = std::bind(prefixCommand, std::ref(dicts), _1, std::ref(std::cout));
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
