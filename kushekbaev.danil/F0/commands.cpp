#include "commands.hpp"
#include <stdexcept>
#include <vector>
#include <fstream>

using dictionary_system = std::unordered_map< std::string, std::unordered_map< std::string, std::set < std::string > > >;

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
  }
  auto& word_map = dict_it->second;
  for (auto word_it = word_map.begin(); word_it != word_map.end(); ++word_it)
  {
    const std::string& word = word_it->first;
    const std::set< std::string >& translations = word_it->second;
    out << std::string("-> ") << word << std::string(" : ") << *translations.begin();
    for (auto translation_it = ++translations.begin(); translation_it != translations.end(); ++translation_it)
    {
      out << std::string(", ") << *translation_it;
    }
    out << std::string("\n");
  }
}

void kushekbaev::remove(std::ostream& out, std::istream& in, dictionary_system& current_dictionary)
{
  std::string dictionary_name, word_to_remove;
  in >> dictionary_name >> word_to_remove;
  auto dict_it = current_dictionary.find(dictionary_name);
  if (dict_it == current_dictionary.end())
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

void kushekbaev::save(std::ostream& out, std::istream& in, dictionary_system& current_dictionary)
{
  std::string filename;
  in >> filename;
  std::ofstream file(filename);
  if (!file.is_open())
  {
    throw std::runtime_error("Cannot open/create your file!");
  }
  for (const auto& dict_pair: current_dictionary)
  {
    const std::string& dict_name = dict_pair.first;
    const auto& word_map = dict_pair.second;
    file << "[ " << dict_name << " ]\n";
    for (const auto& word_pair: word_map)
    {
      const std::string& word = word_pair.first;
      const std::set< std::string >& translations = word_pair.second;
      file << std::string("*") << word << std::string("*");
      for (const auto& translation: translations)
      {
        file << " " << std::string("|") << translation << std::string("|");
      }
      file << "\n";
    }
    file << "\n";
  }
  out << "Dictionary system successfully saved.\n";
}

void kushekbaev::import_dictionary(std::ostream& out, std::istream& in, dictionary_system& current_dictionary)
{
  std::string filename;
    in >> filename;
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open your file!");
    }

    std::string line;
    std::string current_dictionary_name;
    bool in_dictionary = false;

    while (std::getline(file, line))
    {
        // Удаляем начальные и конечные пробелы
        size_t start_pos = line.find_first_not_of(" \t\r\n");
        if (start_pos == std::string::npos) {
            in_dictionary = false;
            continue;
        }
        size_t end_pos = line.find_last_not_of(" \t\r\n");
        std::string trimmed_line = line.substr(start_pos, end_pos - start_pos + 1);

        // Обработка заголовка словаря
        if (trimmed_line.size() > 4 && 
            trimmed_line[0] == '[' && 
            trimmed_line[1] == ' ' &&
            trimmed_line[trimmed_line.size()-2] == ' ' && 
            trimmed_line[trimmed_line.size()-1] == ']')
        {
            current_dictionary_name = trimmed_line.substr(2, trimmed_line.size()-4);
            in_dictionary = true;
            continue;
        }

        // Обработка строки с переводами
        if (in_dictionary && !current_dictionary_name.empty() && 
            trimmed_line.size() > 2 && 
            trimmed_line[0] == '*' && 
            trimmed_line.find('*', 1) != std::string::npos)
        {
            // Извлекаем слово
            size_t word_end = trimmed_line.find('*', 1);
            std::string word = trimmed_line.substr(1, word_end-1);
            
            // Обрабатываем переводы
            std::set<std::string> translations;
            size_t trans_start = word_end + 1;
            
            while (true) {
                size_t pipe_start = trimmed_line.find('|', trans_start);
                if (pipe_start == std::string::npos) break;
                
                size_t pipe_end = trimmed_line.find('|', pipe_start+1);
                if (pipe_end == std::string::npos) break;
                
                std::string translation = trimmed_line.substr(pipe_start+1, pipe_end-pipe_start-1);
                translations.insert(translation);
                trans_start = pipe_end + 1;
            }

            // Добавляем в словарь
            if (!word.empty() && !translations.empty()) {
                auto& existing_translations = current_dictionary[current_dictionary_name][word];
                existing_translations.insert(translations.begin(), translations.end());
            }
        }
    }
    out << "Successfully imported file.\n";
}

void kushekbaev::search(std::ostream& out, std::istream& in, dictionary_system& current_dictionary)
{
  std::string dictionary_name, word_to_find;
  in >> dictionary_name >> word_to_find;
  auto dict_it = current_dictionary.find(dictionary_name);
  if (dict_it == current_dictionary.end())
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
  out << word_it->first << std::string(" ") << *translations.begin();
  for (auto translation_it = ++translations.begin(); translation_it != translations.end(); ++translation_it)
  {
    out << std::string(" ") << *translation_it;
  }
}

void kushekbaev::clear_dictionary(std::ostream& out, std::istream& in, dictionary_system& current_dictionary)
{
  std::string dictionary_name;
  in >> dictionary_name;
  auto dict_it = current_dictionary.find(dictionary_name);
  if (dict_it == current_dictionary.end())
  {
    throw std::out_of_range("<DICTIONARY NOT FOUND>");
  }
  current_dictionary.erase(dict_it);
  out << "Dictionary successfully deleted\n";
}

void kushekbaev::print_help(std::ostream& out)
{
  out << std::string("Available commands: insert, ...\n");
}
