#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>
#include <string>
#include <set>
#include <unordered_map>

namespace kushekbaev
{
  using dictionary_system = std::unordered_map< std::string, std::unordered_map< std::string, std::set < std::string > > >;

  void insert(std::ostream& out, std::istream& in, dictionary_system& current_dictionary);

  void print(std::ostream& out, std::istream& in, dictionary_system& current_dictionary);

  void remove();

  void search();

  void remove_translation_at_all();

  void remove_translation();

  void reverse_search();

  void clear_dictionary();

  void import_dictionary();

  void merge();

  void split();

  void prefix_search();

  void no_prefix_search();

  void suffix_search();

  void no_suffix_search();

  void delete_all_translations();

  void find_words_without_translations();

  void complement();

  void intersect();

  void unification();

  void save();

  void print_list_of_commands(std::ostream& out);
}

#endif
