#include "commands.hpp"

void abramov::getCommands(std::map< std::string, std::function< void() > > &commands, DictionaryCollection &collect)
{
  commands["adddict"] = std::bind(addDict, std::ref(collect), std::ref(std::cin));
  commands["addtodict"] = std::bind(addToDict, std::ref(collect), std::ref(std::cin));
  commands["rmdict"] = std::bind(deleteDict, std::ref(collect), std::ref(std::cin));
  commands["rmfromdict"] = std::bind(deleteFromDict, std::ref(collect), std::ref(std::cin));
  commands["set"] = std::bind(setDicts, std::ref(collect), std::ref(std::cin));
  commands["intersection"] = std::bind(intersectDicts, std::ref(collect), std::ref(std::cin));
  commands["union"] = std::bind(unionDicts, std::ref(collect), std::ref(std::cin));
  commands["difference"] = std::bind(diffDicts, std::ref(collect), std::ref(std::cin));
  commands["merge"] = std::bind(mergeDicts, std::ref(collect), std::ref(std::cin));
  comands["print"] = std::bind(printDicts, std::ref(collect), std::ref(std::cin), std::ref(std::cout));
}

void abramov::addDict(DictionaryCollection &collect, std::istream &in)
{}

void abramov::addToDict(DictionaryCollection &collect, std::istream &in)
{}

void abramov::deleteDict(DictionaryCollection &collect, std::istream &in)
{}

void abramov::deleteFromDict(DictionaryCollection &collect, std::istream &in)
{}

void abramov::setDicts(DictionaryCollection &collect, std::istream &in)
{}

void abramov::intersectDicts(DictionaryCollection &collect, std::istream &in)
{}

void abramov::unionDicts(DictionaryCollection &collect, std::istream &in)
{}

void abramov::diffDicts(DictionaryCollection &collect, std::istream &in)
{}

void abramov::mergeDicts(DictionaryCollection &collect, std::istream &in)
{}

void abramov::printDicts(DictionaryCollection &collect, std::istream &in, std::ostream &out)
{}
