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

  void remove(std::ostream& out, std::istream& in, dictionary_system& current_dictionary);

  void save(std::ostream& out, std::istream& in, dictionary_system& current_dictionary);

  void import_dictionary(std::ostream& out, std::istream& in, dictionary_system& current_dictionary);

  void search(std::ostream& out, std::istream& in, dictionary_system& current_dictionary);

  void clear_dictionary(std::ostream& out, std::istream& in, dictionary_system& current_dictionary);

  void remove_translation_at_all();

  void remove_translation();

  void reverse_search();

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

  void print_help(std::ostream& out);
}

#endif
