#include "commands.hpp"
#include <stdexcept>
#include <functional>
#include <vector>
#include <fstream>
#include "dictionary_utils.hpp"

using dictionary_system = std::unordered_map< std::string, std::unordered_map< std::string, std::set < std::string > > >;
using dict_type = std::unordered_map< std::string, std::set< std::string > >;

void kushekbaev::insert(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name, inputed_word, inputed_translation;
  in >> dictionary_name >> inputed_word >> inputed_translation;
  auto& word_map = current_dictionary_system[dictionary_name];
  auto& translation_set = word_map[inputed_word];
  if (translation_set.count(inputed_translation))
  {
    throw std::logic_error("<THIS TRANSLATION ALREADY EXISTS>");
  }
  translation_set.insert(inputed_translation);
  out << std::string("Translation inserted successfully.\n");
}

void kushekbaev::insert_without_translation(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name, inputed_word;
  in >> dictionary_name >> inputed_word;
  auto& word_map = current_dictionary_system[dictionary_name];
  word_map[inputed_word];
  out << "Successfully inserted without translation!\n";
}

void kushekbaev::print(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name;
  in >> dictionary_name;
  auto dict_it = current_dictionary_system.find(dictionary_name);
  if (dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  auto& word_map = dict_it->second;
  out << "In " << dictionary_name << ":\n";
  if (!word_map.empty())
  {
    DictionaryPrinter printer{out, word_map};
    printer();
  }
}

void kushekbaev::remove(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name, word_to_remove;
  in >> dictionary_name >> word_to_remove;
  auto dict_it = current_dictionary_system.find(dictionary_name);
  if (dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  auto word_it = dict_it->second.find(word_to_remove);
  if (word_it == dict_it->second.end())
  {
    throw std::out_of_range("<WORD NOT FOUND>");
  }
  dict_it->second.erase(word_it);
  out << std::string("Word with its translation successfully removed.\n");
}

void kushekbaev::save(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string filename;
  in >> filename;
  std::ofstream file(filename);
  if (!file.is_open())
  {
    throw std::runtime_error("Cannot open/create your file!");
  }
  if (!current_dictionary_system.empty())
  {
    DictsSaver saver{file, current_dictionary_system};
    saver();
  }
  out << "Dictionary system successfully saved.\n";
}

void kushekbaev::import_dictionary(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string filename;
  in >> filename;
  std::ifstream file(filename);
  if (!file.is_open())
  {
    throw std::runtime_error("Cannot open your file!");
  }
  FileImporter importer{file, current_dictionary_system};
  importer();
  out << "Successfully imported file.\n";
}

void kushekbaev::search(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name, word_to_find;
  in >> dictionary_name >> word_to_find;
  auto dict_it = current_dictionary_system.find(dictionary_name);
  if (dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  auto& word_map = dict_it->second;
  auto word_it = word_map.find(word_to_find);
  if (word_it == word_map.end())
  {
    throw std::out_of_range("<WORD NOT FOUND>");
  }
  auto& translations = word_it->second;
  out << word_it->first;
  if (!translations.empty())
  {
    out << " ";
    TranslationsPrinter printer{out, translations};
    printer();
  }
}

void kushekbaev::clear_dictionary(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name;
  in >> dictionary_name;
  auto dict_it = current_dictionary_system.find(dictionary_name);
  if (dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  current_dictionary_system.erase(dict_it);
  out << "Dictionary successfully deleted\n";
}

void kushekbaev::reverse_search(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name, translation_to_find;
  in >> dictionary_name >> translation_to_find;
  auto dict_it = current_dictionary_system.find(dictionary_name);
  if (dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  std::vector< std::string > matching_words;
  if (!dict_it->second.empty())
  {
    WordCollector collector{ translation_to_find, matching_words, dict_it->second };
    collector();
  }
  if (matching_words.empty())
  {
    throw std::out_of_range("<TRANSLATION NOT FOUND>");
  }
  out << "Words with translation *" << translation_to_find << "*: ";
  WordsPrinter printer{out, matching_words};
  printer();
  out << "\n";
}

void kushekbaev::remove_translation(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name, word_to_find, translation_to_delete;
  in >> dictionary_name >> word_to_find >> translation_to_delete;
  auto dict_it = current_dictionary_system.find(dictionary_name);
  if (dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  auto& word_map = dict_it->second;
  auto word_it = word_map.find(word_to_find);
  if (word_it == word_map.end())
  {
    throw std::out_of_range("<WORD NOT FOUND>");
  }
  auto& translations = word_it->second;
  auto translation_it = translations.find(translation_to_delete);
  if (translation_it == translations.end())
  {
    throw std::out_of_range("<TRANSLATION NOT FOUND>");
  }
  translations.erase(translation_it);
  out << std::string("Translation successfully deleted.\n");
}

void kushekbaev::remove_translation_at_all(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name, translation_to_delete;
    in >> dictionary_name >> translation_to_delete;
    auto dict_it = current_dictionary_system.find(dictionary_name);
    if (dict_it == current_dictionary_system.end())
    {
      throw std::out_of_range("<DICTIONARY NOT FOUND>");
    }
    std::vector< std::string > words_to_erase;
    size_t removed_count = 0;
    if (!dict_it->second.empty())
    {
      TranslationRemover remover{ translation_to_delete, words_to_erase, removed_count, dict_it->second };
      remover();
    }
    if (removed_count == 0)
    {
        throw std::out_of_range("<TRANSLATION NOT FOUND>");
    }
  out << "Removed " << removed_count << " instances of translation '" << translation_to_delete << ".'\n";
}

void kushekbaev::delete_all_translations(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name, translation_to_delete;
    in >> dictionary_name >> translation_to_delete;
    auto dict_it = current_dictionary_system.find(dictionary_name);
    if (dict_it == current_dictionary_system.end())
    {
        throw std::out_of_range("<DICTIONARY NOT FOUND>");
    }
    std::vector< std::string > matching_words;
    if (!dict_it->second.empty())
    {
      WordFinder finder{ translation_to_delete, matching_words, dict_it->second };
      finder();
    }
    if (matching_words.empty())
    {
        throw std::out_of_range("<TRANSLATION NOT FOUND>");
    }
    if (!matching_words.empty())
    {
      for (const auto& word : matching_words)
      {
        dict_it->second.erase(word);
      }
    }
  out << std::string("Words with this translation deleted successfully.\n");
}

void kushekbaev::prefix_search(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name, prefix;
  in >> dictionary_name >> prefix;
  auto dict_it = current_dictionary_system.find(dictionary_name);
  if (dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  bool found = false;
  out << "Words with prefix '" << prefix << "':\n";
  for (const auto& word_pair: dict_it->second)
  {
    const std::string& word = word_pair.first;
    if (word.rfind(prefix, 0) == 0)
    {
      out << "-> " << word << " : ";
      const auto& translations = word_pair.second;
      for (auto it = translations.begin(); it != translations.end(); ++it)
      {
        if (it != translations.begin()) out << ", ";
        out << *it;
      }
      out << "\n";
      found = true;
    }
  }
  if (!found)
  {
    out << "<NO WORDS FOUND>\n";
  }
}

void kushekbaev::no_prefix_search(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name, prefix;
  in >> dictionary_name >> prefix;
  auto dict_it = current_dictionary_system.find(dictionary_name);
  if (dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  bool found = false;
  out << "Words without prefix '" << prefix << "':\n";
  for (const auto& word_pair: dict_it->second)
  {
    const std::string& word = word_pair.first;
    if (word.rfind(prefix, 0) != 0)
    {
      out << "-> " << word << " : ";
      const auto& translations = word_pair.second;
      for (auto it = translations.begin(); it != translations.end(); ++it)
      {
        if (it != translations.begin()) out << ", ";
        out << *it;
      }
      out << "\n";
      found = true;
    }
  }
  if (!found)
  {
    out << "<NO WORDS FOUND>\n";
  }
}

void kushekbaev::suffix_search(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name, suffix;
  in >> dictionary_name >> suffix;
  auto dict_it = current_dictionary_system.find(dictionary_name);
  if (dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  bool found = false;
  out << "Words with suffix '" << suffix << "':\n";
  for (const auto& word_pair: dict_it->second)
  {
    const std::string& word = word_pair.first;
    if (word.size() >= suffix.size() && word.compare(word.size() - suffix.size(), suffix.size(), suffix) == 0)
    {
      out << "-> " << word << " : ";
      const auto& translations = word_pair.second;
      for (auto it = translations.begin(); it != translations.end(); ++it)
      {
        if (it != translations.begin()) out << ", ";
        out << *it;
      }
      out << "\n";
      found = true;
    }
  }
  if (!found)
  {
    out << "<NO WORDS FOUND>\n";
  }
}

void kushekbaev::no_suffix_search(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary_name, suffix;
  in >> dictionary_name >> suffix;
  auto dict_it = current_dictionary_system.find(dictionary_name);
  if (dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  bool found = false;
  out << "Words with bo suffix '" << suffix << "':\n";
  for (const auto& word_pair: dict_it->second)
  {
    const std::string& word = word_pair.first;
    if (word.size() >= suffix.size() && word.compare(word.size() - suffix.size(), suffix.size(), suffix) != 0)
    {
      out << "-> " << word << " : ";
      const auto& translations = word_pair.second;
      for (auto it = translations.begin(); it != translations.end(); ++it)
      {
        if (it != translations.begin()) out << ", ";
        out << *it;
      }
      out << "\n";
      found = true;
    }
  }
  if (!found)
  {
    out << "<NO WORDS FOUND>\n";
  }
}

void kushekbaev::merge(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string new_dictionary, first_dictionary, second_dictionary;
  in >> new_dictionary >> first_dictionary >> second_dictionary;
  auto first_dict_it = current_dictionary_system.find(first_dictionary);
  if (first_dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<FIRST DICTIONARY NOT FOUND>");
  }
  auto second_dict_it = current_dictionary_system.find(second_dictionary);
  if (second_dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<SECOND DICTIONARY NOT FOUND>");
  }
  if (current_dictionary_system.count(new_dictionary))
  {
    throw std::runtime_error("<NEW DICTIONARY NAME ALREADY EXISTS>");
  }
  auto& new_dict = current_dictionary_system[new_dictionary];
  for (const auto& entry : first_dict_it->second)
  {
    new_dict[entry.first] = entry.second;
  }
  for (const auto& entry : second_dict_it->second)
  {
    for (const auto& translation : entry.second)
    {
      new_dict[entry.first].insert(translation);
    }
  }
  out << "Successfully merged!\n";
}

void kushekbaev::split(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string new_dictionary_1, new_dictionary_2, original_dictionary, delimiter;
  in >> new_dictionary_1 >> new_dictionary_2 >> original_dictionary >> delimiter;
  auto original_dict_it = current_dictionary_system.find(original_dictionary);
  if (original_dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<ORIGINAL DICTIONARY NOT FOUND>");
  }
  if (current_dictionary_system.count(new_dictionary_1) || current_dictionary_system.count(new_dictionary_2))
  {
    throw std::runtime_error("<NEW DICTIONARY NAME ALREADY EXISTS>");
  }
  auto& dict1 = current_dictionary_system[new_dictionary_1];
  auto& dict2 = current_dictionary_system[new_dictionary_2];
  for (const auto& entry : original_dict_it->second)
  {
    const std::string& word = entry.first;
    const std::set< std::string >& translations = entry.second;
    if (word < delimiter)
    {
      dict1[word] = translations;
    }
    else
    {
      dict2[word] = translations;
    }
  }
  out << "Successfully splitted.\n";
}

void kushekbaev::find_words_without_translations(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string dictionary;
  in >> dictionary;
  auto dict_it = current_dictionary_system.find(dictionary);
  if (dict_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  bool found_empty = false;
  out << "Words without translations in dictionary '" << dictionary << "':\n";
  for (const auto& word_entry : dict_it->second)
  {
    const std::string& word = word_entry.first;
    const std::set< std::string >& translations = word_entry.second;
    if (translations.empty())
    {
      out << "-> " << word << "\n";
      found_empty = true;
    }
  }
  if (!found_empty)
  {
    out << "<ALL WORDS HAVE TRANSLATIONS>\n";
  }
}

void kushekbaev::complement(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
    std::string new_name, dict1_name, dict2_name;
    in >> new_name >> dict1_name >> dict2_name;
    auto dict1_it = current_dictionary_system.find(dict1_name);
    auto dict2_it = current_dictionary_system.find(dict2_name);
    if (dict1_it == current_dictionary_system.end() || dict2_it == current_dictionary_system.end())
    {
      throw std::out_of_range("<DICTIONARY NOT FOUND>");
    }
    if (dict1_name == dict2_name)
    {
      current_dictionary_system[new_name] = {};
      out << "Created empty dictionary (same dictionaries complemented)\n";
      return;
    }
    auto& new_dict = current_dictionary_system[new_name] = {};
    const auto& dict1 = dict1_it->second;
    const auto& dict2 = dict2_it->second;
    if (!dict1.empty())
    {
      ComplementWorker worker{new_dict, dict1, dict2};
      worker();
    }
    out << "Complement dictionary created with " << new_dict.size() << " words\n";
}

void kushekbaev::intersect(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
    std::string new_name, dict1_name, dict2_name;
    in >> new_name >> dict1_name >> dict2_name;
    auto dict1_it = current_dictionary_system.find(dict1_name);
    auto dict2_it = current_dictionary_system.find(dict2_name);
    if (dict1_it == current_dictionary_system.end() || dict2_it == current_dictionary_system.end())
    {
      throw std::out_of_range("<DICTIONARY NOT FOUND>");
    }
    auto& new_dict = current_dictionary_system[new_name] = {};
    const auto& dict1 = dict1_it->second;
    const auto& dict2 = dict2_it->second;
    if (dict1_name == dict2_name)
    {
      new_dict = dict1;
      out << "Dictionary copied (same dictionaries intersected)\n";
      return;
    }
    if (!dict1.empty())
    {
      IntersectWorker worker{new_dict, dict1, dict2};
      worker();
    }
    out << "Intersection created with " << new_dict.size() << " common words\n";
}

void kushekbaev::unification(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system)
{
  std::string new_name, dict1_name, dict2_name;
  in >> new_name >> dict1_name >> dict2_name;

  auto dict1_it = current_dictionary_system.find(dict1_name);
  auto dict2_it = current_dictionary_system.find(dict2_name);
  if (dict1_it == current_dictionary_system.end() || dict2_it == current_dictionary_system.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  if (current_dictionary_system.count(new_name))
  {
    throw std::runtime_error("<NEW DICTIONARY NAME ALREADY EXISTS>");
  }
  auto& new_dict = current_dictionary_system[new_name];
  const auto& dict1 = dict1_it->second;
  const auto& dict2 = dict2_it->second;
  for (const auto& entry : dict1)
  {
    new_dict[entry.first] = entry.second;
  }
  for (const auto& entry : dict2)
  {
    for (const auto& translation : entry.second)
    {
      new_dict[entry.first].insert(translation);
    }
  }
  out << "Unification dictionary created successfully\n";
}

void kushekbaev::print_help(std::ostream& out)
{
  out << std::string("Available commands: insert, ...\n");
}
