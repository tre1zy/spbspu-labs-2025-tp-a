#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>
#include <string>
#include <set>
#include <unordered_map>

namespace kushekbaev
{
  using dictionary_system = std::unordered_map< std::string, std::unordered_map< std::string, std::set < std::string > > >;

  void insert(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system);

  void print(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system);

  void remove(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system);

  void save(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system);

  void import_dictionary(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system);

  void search(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system);

  void clear_dictionary(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system);

  void reverse_search(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system);

  void remove_translation(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system);

  void remove_translation_at_all(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system);

  void delete_all_translations(std::ostream& out, std::istream& in, dictionary_system& current_dictionary_system);

  void merge();

  void split();

  void prefix_search();

  void no_prefix_search();

  void suffix_search();

  void no_suffix_search();


  void find_words_without_translations();

  void complement();

  void intersect();

  void unification();

  void print_help(std::ostream& out);
}

#endif
