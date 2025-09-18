#include "dictionary_utils.hpp"
#include <algorithm>
#include <iterator>

std::vector< std::string > kushekbaev::split_line(std::string& line)
{
  std::vector< std::string > tokens;
  size_t pos = 0;
  while (true)
  {
    size_t start = line.find_first_not_of(" \t", pos);
    if (start == std::string::npos)
    {
      break;
    }
    size_t end = line.find_first_of(" \t", start);
    if (end == std::string::npos)
    {
      end = line.size();
    }
    tokens.push_back(line.substr(start, end - start));
    pos = end;
  }
  return tokens;
}

void kushekbaev::DictionaryMergerHelper::operator()(const std::pair< const std::string, std::set< std::string > >& entry)
{
  target[entry.first].insert(entry.second.begin(), entry.second.end());
}

void kushekbaev::EntryPrinter::operator()(const std::pair<const std::string, std::set< std::string > >& entry)
{
  const std::string& word = entry.first;
  const std::set< std::string >& translations = entry.second;
  out << "  -> " << word << " : ";
  if (translations.empty())
  {
    out << "<NO TRANSLATIONS>";
  }
  else
  {
    TranslationsPrinter printer{out, translations};
    printer();
  }
  out << "\n";
}

void kushekbaev::WordEntrySaver::operator()(const std::pair< const std::string, std::set< std::string > >& entry)
{
  const std::string& word = entry.first;
  const std::set< std::string >& translations = entry.second;
  file << word;
  if (!translations.empty())
  {
    TranslationsSaver saver{ file, translations };
    saver();
  }
  file << "\n";
}

void kushekbaev::DictionaryEntrySaver::operator()(const std::pair< const std::string, dict_type >& dict_entry)
{
  const std::string& dict_name = dict_entry.first;
  const auto& word_map = dict_entry.second;
  file << "[ " << dict_name << " ]\n";
  if (!word_map.empty())
  {
    WordsSaver saver{ file, word_map };
    saver();
  }
  file << "\n";
}

void kushekbaev::WordEraserHelper::operator()(const std::string& word)
{
  word_map.erase(word);
}

bool kushekbaev::IsNotInDict2::operator()(const std::pair<const std::string, std::set< std::string > >& entry)
{
  return dict2.find(entry.first) == dict2.end();
}

bool kushekbaev::HasTranslationPredicate::operator()(const std::pair< const std::string, std::set< std::string > >& entry)
{
  const std::set< std::string >& translations = entry.second;
  return translations.find(translation_to_delete) != translations.end();
}

const std::string& kushekbaev::KeyExtractor::operator()(const std::pair< const std::string, std::set< std::string > >& entry)
{
  return entry.first;
}

void kushekbaev::IntersectInserter::operator()(const std::pair< const std::string, std::set< std::string > >& entry)
{
  const std::string& word = entry.first;
  auto dict2_it = dict2.find(word);
  if (dict2_it != dict2.end())
  {
    auto& new_translations = new_dict[word];
    new_translations.insert(entry.second.begin(), entry.second.end());
    new_translations.insert(dict2_it->second.begin(), dict2_it->second.end());
  }
}

void kushekbaev::DictionaryMerger::operator()(const dict_type& source)
{
  std::for_each(source.begin(), source.end(), DictionaryMergerHelper{ target });
}

void kushekbaev::DictionaryInserter::operator()(const dictionary_system::value_type& entry)
{
  target[entry.first].insert(entry.second.begin(), entry.second.end());
}

void kushekbaev::TranslationSaver::operator()(const std::string& translation)
{
  file << " " << translation;
}

void kushekbaev::TranslationsPrinter::operator()()
{
  if (!translations.empty())
  {
    out << *translations.begin();
    std::copy(std::next(translations.begin()), translations.end(), std::ostream_iterator< std::string >(out, " "));
  }
}

void kushekbaev::DictionaryPrinter::operator()()
{
  std::for_each(word_map.begin(), word_map.end(), EntryPrinter{ out });
}

void kushekbaev::TranslationsSaver::operator()()
{
  std::for_each(translations.begin(), translations.end(), TranslationSaver{ file });
}

void kushekbaev::WordsSaver::operator()()
{
  std::for_each(word_map.begin(), word_map.end(), WordEntrySaver{ file });
}

void kushekbaev::DictsSaver::operator()()
{
  std::for_each(dicts.begin(), dicts.end(), DictionaryEntrySaver{file});
}

void kushekbaev::insert_translations(std::set< std::string >& translations, const std::vector< std::string >& tokens, size_t index)
{
  if (index  >= tokens.size())
  {
    return;
  }
  translations.insert(tokens[index]);
  insert_translations(translations, tokens, index + 1);
}

void kushekbaev::FileImporter::operator()()
{
  std::string line;
  while (std::getline(file, line))
  {
    ++line_count;
    auto start = line.find_first_not_of(" \r\n");
    if (start == std::string::npos)
    {
      in_dict = false;
      continue;
    }
    auto end = line.find_last_not_of(" \r\n");
    std::string trimmed = line.substr(start, end - start + 1);
    if (trimmed.size()  > 2 && trimmed[0] == '[' && trimmed[trimmed.size()-1] == ']')
    {
      current_dict_name = trimmed.substr(1, trimmed.size()-2);
      start = current_dict_name.find_first_not_of(" ");
      end = current_dict_name.find_last_not_of(" ");
      if (start != std::string::npos)
      {
        current_dict_name = current_dict_name.substr(start, end - start + 1);
      }
      if (current_dict_name.empty())
      {
        throw std::runtime_error("Empty dictionary name at line " + std::to_string(line_count));
      }
      in_dict = true;
      continue;
    }
    if (in_dict && !current_dict_name.empty() && !trimmed.empty())
    {
      auto tokens = split_line(trimmed);
      if (!tokens.empty())
      {
        std::string word = tokens[0];
        std::set< std::string > translations;
        if (tokens.size()  > 1)
        {
          for (size_t i = 1; i <  tokens.size(); ++i)
          {
            translations.insert(tokens[i]);
          }
        }
        dicts[current_dict_name][word].insert(translations.begin(), translations.end());
      }
    }
  }
}

void kushekbaev::WordCollector::operator()()
{
  for (const auto& entry : word_map)
  {
    const std::string& word = entry.first;
    const std::set< std::string >& translations = entry.second;
    if (translations.find(translation_to_find) != translations.end())
    {
      matching_words.push_back(word);
    }
  }
}

void kushekbaev::WordsPrinter::operator()()
{
  if (!words.empty())
  {
    out << words[0];
    std::copy(std::next(words.begin()), words.end(), std::ostream_iterator< std::string >(out, ", "));
  }
}

void kushekbaev::TranslationRemover::operator()()
{
  for (auto it = word_map.begin(); it != word_map.end(); ++it)
  {
    auto& translations = it->second;
    if (translations.erase(translation_to_delete))
    {
      removed_count++;
      if (translations.empty())
      {
        words_to_erase.push_back(it->first);
        it = word_map.erase(it);
        continue;
      }
    }
  }
}

void kushekbaev::WordEraser::operator()()
{
  std::for_each(words_to_erase.begin(), words_to_erase.end(), WordEraserHelper{ word_map });
}

void kushekbaev::WordFinder::operator()()
{
  std::vector< std::pair< const std::string, std::set< std::string> > > matching_entries;
  HasTranslationPredicate predicate{translation_to_delete};
  std::copy_if(word_map.begin(), word_map.end(), std::back_inserter(matching_entries), predicate);
  KeyExtractor extractor;
  std::transform(matching_entries.begin(), matching_entries.end(), std::back_inserter(matching_words), extractor);
}

void kushekbaev::ComplementWorker::operator()()
{
  IsNotInDict2 predicate{ dict2 };
  std::copy_if(dict1.begin(), dict1.end(), std::inserter(new_dict, new_dict.end()), predicate);
}

void kushekbaev::IntersectWorker::operator()()
{
  std::for_each(dict1.begin(), dict1.end(), IntersectInserter{new_dict, dict2});
}
