#include <map>
#include <string>
#include <limits>
#include <fstream>
#include <iostream>
#include <functional>
#include <unordered_map>
#include "commands.hpp"

int main(int argc, char * argv[])
{
  using namespace cherkasov;
  DictTable dicts;

  if (argc == 2)
  {
    if (std::string(argv[1]) == "--help")
    {
      std::cout << "Usage: ./program [text_file]\n";
      std::cout << "Commands:\n";
      std::cout << " createdict <dict>\n showdicts\n loadtext <dict> <file>\n"
                   " union <res> <d1> <d2>\n intersect <res> <d1> <d2>\n copy <res> <dict>\n"
                   " addword <dict> <word> <freq>\n size <dict>\n cleanword <dict> <word>\n"
                   " cleandict <dict>\n top <dict> <n>\n rare <dict> <n>\n";
      return 0;
    }
    try
    {
      std::ifstream fin(argv[1]);
      if (!fin.is_open())
      {
        throw std::runtime_error("<INVALID FILE>");
      }
      Dict d;
      std::string word;
      while (fin >> word)
      {
        ++d[word];
      }
      dicts["default"] = std::move(d);
    }
    catch (const std::exception & e)
    {
      std::cerr << e.what() << '\n';
      return 1;
    }
  }
  std::map< std::string, std::function< void() > > cmds;
  cmds["createdict"]  = std::bind(makeDict, std::ref(std::cin), std::ref(dicts));
  cmds["showdicts"]   = std::bind(listDicts, std::ref(std::cout), std::cref(dicts));
  cmds["loadtext"]    = std::bind(importText, std::ref(std::cin), std::ref(dicts));
  cmds["union"]       = std::bind(mergeDicts, std::ref(std::cin), std::ref(dicts));
  cmds["intersect"]   = std::bind(intersectDicts, std::ref(std::cin), std::ref(dicts));
  cmds["copy"]        = std::bind(cloneDict, std::ref(std::cin), std::ref(dicts));
  cmds["addword"]     = std::bind(insertWord, std::ref(std::cin), std::ref(dicts));
  cmds["size"]        = std::bind(dictSize, std::ref(std::cin), std::ref(std::cout), std::cref(dicts));
  cmds["cleanword"]   = std::bind(eraseWord, std::ref(std::cin), std::ref(dicts));
  cmds["cleandict"]   = std::bind(clearDict, std::ref(std::cin), std::ref(dicts));
  cmds["top"]         = std::bind(printTop, std::ref(std::cin), std::ref(std::cout), std::cref(dicts));
  cmds["rare"]        = std::bind(printRare, std::ref(std::cin), std::ref(std::cout), std::cref(dicts));
  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      cmds.at(command)();
    }
    catch (const std::out_of_range &)
    {
      std::cout << "<INVALID COMMAND>" << '\n';
    }
    catch (const std::exception & e)
    {
      std::cout << e.what() << '\n';
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
}
