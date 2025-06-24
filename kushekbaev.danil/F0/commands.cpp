#include "commands.hpp"
#include <stdexcept>

void kushekbaev::insert(std::ostream& out, std::istream& in, dictionary_system& current_dictionary)
{
  std::string dictionary_name, inputed_word, inputed_translation;
  in >> dictionary_name >> inputed_word >> inputed_translation;
  auto& word_map = current_dictionary[dictionary_name];
  auto& translation_set = word_map[inputed_word];
  if (translation_set.count(inputed_translation))
  {
    throw std::logic_error("<THIS TRANSLATION ALREADY EXISTS>");
  }
  translation_set.insert(inputed_translation);
  out << std::string("Translation inserted successfully.\n");
}

void kushekbaev::print(std::ostream& out, std::istream& in, dictionary_system& current_dictionary)
{
  std::string dictionary_name;
  in >> dictionary_name;
  auto dict_it = current_dictionary.find(dictionary_name);
  if (dict_it == current_dictionary.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
    return;
  }
  auto& word_map = dict_it->second;
  for (auto word_it = word_map.begin(); word_it != word_map.end(); ++word_it)
  {
    const std::string& word = word_it->first;
    const std::set< std::string >& translations = word_it->second;
    out << word << std::string(" ") << *translations.begin();
    for (auto translation_it = ++translations.begin(); translation_it != translations.end(); ++translation_it)
    {
      out << std::string(" ") << *translation_it;
    }
  }
}

void kushekbaev::print_list_of_commands(std::ostream& out)
{
  out << std::string("Available commands: insert, ...\n");
}
