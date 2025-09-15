#ifndef DICTIONARY_UTILS_HPP
#define DICTIONARY_UTILS_HPP

#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include <fstream>
#include <iostream>

using dictionary_system = std::unordered_map< std::string, std::unordered_map< std::string, std::set< std::string > > >;
using dict_type = std::unordered_map< std::string, std::set< std::string > >;

namespace kushekbaev
{

std::vector< std::string > split_line(std::string& line);

struct DictionaryMergerHelper
{
  dict_type& target;
  void operator()(const std::pair< const std::string, std::set< std::string > >& entry);
};

struct EntryPrinter
{
  std::ostream& out;
  void operator()(const std::pair< const std::string, std::set< std::string > >& entry);
};

struct TranslationSaver
{
  std::ofstream& file;
  void operator()(const std::string& translation);
};

struct WordEntrySaver
{
  std::ofstream& file;
  void operator()(const std::pair< const std::string, std::set< std::string > >& entry);
};

struct WordEraserHelper
{
  dict_type& word_map;
  void operator()(const std::string& word);
};

struct DictionaryEntrySaver
{
  std::ofstream& file;
  void operator()(const std::pair< const std::string, dict_type >& dict_entry);
};

struct IsNotInDict2
{
  const dict_type& dict2;
  bool operator()(const std::pair<const std::string, std::set<std::string>>& entry);
};

struct DictionaryMerger
{
  dict_type& target;
  void operator()(const dict_type& source);
};

struct DictionaryInserter
{
  dictionary_system& target;
  void operator()(const dictionary_system::value_type& entry);
};

struct HasTranslationPredicate
{
  const std::string& translation_to_delete;
  bool operator()(const std::pair< const std::string, std::set< std::string > >& entry);
};

struct KeyExtractor
{
  const std::string& operator()(const std::pair< const std::string, std::set< std::string > >& entry);
};

struct IntersectInserter
{
  dict_type& new_dict;
  const dict_type& dict2;
  void operator()(const std::pair< const std::string, std::set< std::string > >& entry);
};

struct TranslationsPrinter
{
  std::ostream& out;
  const std::set< std::string >& translations;
  bool first = true;
  void operator()();
};

struct DictionaryPrinter
{
  std::ostream& out;
  const dict_type& word_map;
  void operator()();
};

struct TranslationsSaver
{
  std::ofstream& file;
  const std::set< std::string >& translations;
  void operator()();
};

struct WordsSaver
{
  std::ofstream& file;
  const dict_type& word_map;
  void operator()();
};

struct DictsSaver
{
  std::ofstream& file;
  const dictionary_system& dicts;
  void operator()();
};

void insert_translations(std::set< std::string >& translations, const std::vector< std::string >& tokens, size_t index);

struct FileImporter
{
  std::ifstream& file;
  dictionary_system& dicts;
  std::string current_dict_name;
  bool in_dict = false;
  size_t line_count = 0;
  void operator()();
};

struct WordCollector
{
  const std::string& translation_to_find;
  std::vector< std::string >& matching_words;
  const dict_type& word_map;
  void operator()();
};

struct WordsPrinter
{
  std::ostream& out;
  const std::vector< std::string >& words;
  void operator()();
};

struct TranslationRemover
{
  const std::string& translation_to_delete;
  std::vector< std::string >& words_to_erase;
  size_t& removed_count;
  dict_type& word_map;
  void operator()();
};

struct WordEraser
{
  dict_type& word_map;
  const std::vector< std::string >& words_to_erase;
  void operator()();
};

struct WordFinder
{
  const std::string& translation_to_delete;
  std::vector< std::string >& matching_words;
  const dict_type& word_map;
  void operator()();
};

struct ComplementWorker
{
  dict_type& new_dict;
  const dict_type& dict1;
  const dict_type& dict2;
  void operator()();
};

struct IntersectWorker
{
  dict_type& new_dict;
  const dict_type& dict1;
  const dict_type& dict2;
  void operator()();
};

}

#endif