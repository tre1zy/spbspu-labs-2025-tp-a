#include <map>
#include <string>
#include <limits>
#include <functional>
#include <iostream>
#include "commands.hpp"

int main(int argc, char* argv[])
{
  using dictionary_system = std::unordered_map< std::string, std::unordered_map< std::string, std::set < std::string > > >;
  using namespace kushekbaev;
  if (argc == 2 && std::string(argv[1]) == "--help")
  {
    print_list_of_commands(std::cout);
  }
  dictionary_system current_dictionary;
  std::map< std::string, std::function<void() > > commands;
  commands["insert"] = std::bind(insert, std::ref(std::cout), std::ref(std::cin), std::ref(current_dictionary));
  commands["print"] = std::bind(print, std::ref(std::cout), std::ref(std::cin), std::ref(current_dictionary));
/*  commands["remove"] = std::bind(max, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["search"] = std::bind(min, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["remove_translation_at_all"] = std::bind(count, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["remove_translation"] = std::bind(rightshapes, std::ref(std::cout), std::cref(polygons));
  commands["reverse_search"] = std::bind(same, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["clear_dictionary"] = std::bind(area, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["import_dictionary"] = std::bind(max, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["merge"] = std::bind(min, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["split"] = std::bind(count, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["prefix_search"] = std::bind(rightshapes, std::ref(std::cout), std::cref(polygons));
  commands["no_prefix_search"] = std::bind(same, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["suffix_search"] = std::bind(area, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["no_suffix_search"] = std::bind(max, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["delete_all_translations"] = std::bind(min, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["find_words_without_translation"] = std::bind(count, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["complement"] = std::bind(same, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["intersect"] = std::bind(rightshapes, std::ref(std::cout), std::cref(polygons));
  commands["unification"] = std::bind(same, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  commands["save"] = std::bind(rightshapes, std::ref(std::cout), std::cref(polygons)); */

  std::string command;
  while (std::cin >> command)
  {
    try
    {
      commands.at(command)();
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
