#include "commands.hpp"
#include <stdexcept>

void kushekbaev::insert(std::ostream& out, std::istream& in, dictionary_system& current_dictionary)
{
  std::string dictionary_name, inputed_word, inputed_translation;
  in >> dictionary_name >> inputed_word >> inputed_translation;
  auto dict_it = current_dictionary.find(dictionary_name);
  if (dict_it == current_dictionary.end())
  {
    throw std::out_of_range("Empty structure!");
    return;
  }
  auto& word_in_dictionary = dict_it->second;
  auto word_it = word_in_dictionary.find(inputed_word);
  if (word_it != word_in_dictionary.end())
  {
    if (word_it->second.count(inputed_translation))
    {
      throw std::logic_error("<THIS TRANSLATION ALREADY EXIST>");
      return;
    }
  }
  word_in_dictionary[inputed_word].insert(inputed_translation);
  out << std::string("Перевод успешно вставлен.\n");
}

kushekbaev::print_list_of_commands(std::ostream& out)
{
  out << std::string("z");
}
