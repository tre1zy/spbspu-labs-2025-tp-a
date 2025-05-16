#include <iostream>
#include <unordered_map>
#include <functional>
#include "FrequencyDictionary.hpp"

int main()
{
  shapkov::FrequencyDictionary dict;
  std::unordered_map< std::string, std::function< void() > > cmds;
  cmds["ANALYZETEXT"] = std::bind(shapkov::analyze_text, std::ref(std::cin), std::ref(std::cout), std::ref(dict));
  cmds["PRINT"] = std::bind(shapkov::print, std::ref(std::cin), std::ref(std::cout), std::cref(dict));
  cmds["WORDINFO"] = std::bind(shapkov::word_info, std::ref(std::cin), std::ref(std::cout), std::cref(dict));
  cmds["ANAGRAMS"] = std::bind(shapkov::anagrams, std::ref(std::cin), std::ref(std::cout), std::cref(dict));
  cmds["SIMILARFREQ"] = std::bind(shapkov::similar_frequency, std::ref(std::cin), std::ref(std::cout), std::cref(dict));
  cmds["ENTROPY"] = std::bind(shapkov::entropy, std::ref(std::cin), std::ref(std::cout), std::cref(dict));
  cmds["TOTALWORDS"] = std::bind(shapkov::total_words, std::ref(std::cin), std::ref(std::cout), std::cref(dict));
  cmds["PALINDROMES"] = std::bind(shapkov::palindromes, std::ref(std::cin), std::ref(std::cout), std::cref(dict));
  cmds["TOP"] = std::bind(shapkov::top, std::ref(std::cin), std::ref(std::cout), std::cref(dict));
  cmds["PATTERN"] = std::bind(shapkov::show_with_pattern, std::ref(std::cin), std::ref(std::cout), std::cref(dict));
  cmds["COMPARE"] = std::bind(shapkov::compare, std::ref(std::cin), std::ref(std::cout), std::cref(dict));
  cmds["INTERSECT"] = std::bind(shapkov::intersect, std::ref(std::cin), std::ref(std::cout), std::ref(dict));
  cmds["DIFF"] = std::bind(shapkov::diff, std::ref(std::cin), std::ref(std::cout), std::ref(dict));
  cmds["MERGE"] = std::bind(shapkov::merge, std::ref(std::cin), std::ref(std::cout), std::ref(dict));

  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      cmds.at(command)();
    }
    catch (...)
    {
      if (std::cin.fail())
      {
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
      }
  }
  return 0;
}
