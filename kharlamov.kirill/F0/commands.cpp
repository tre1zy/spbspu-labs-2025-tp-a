#include "commands.h"
#include <algorithm>
#include <cctype>
#include <functional>
#include <fstream>
#include <iterator>
#include <numeric>

namespace kharlamov
{

  void Dictionary::insert(const std::string& key, const std::string& value)
  {
    data[key] = value;
  }

  bool Dictionary::contains(const std::string& key) const
  {
    return data.find(key) != data.end();
  }

  std::string Dictionary::translate(const std::string& key) const
  {
    auto it = data.find(key);
    return it != data.end() ? it->second : "";
  }

  bool Dictionary::remove(const std::string& key)
  {
    return data.erase(key) > 0;
  }

  void Dictionary::clear()
  {
    data.clear();
  }

  size_t Dictionary::size() const
  {
    return data.size();
  }

  bool Dictionary::empty() const
  {
    return data.empty();
  }

  std::vector<std::pair<std::string, std::string>> Dictionary::getAllWords() const
  {
    std::vector<std::pair<std::string, std::string>> result;
    std::copy(data.begin(), data.end(), std::back_inserter(result));
    return result;
  }

  std::vector<std::pair<std::string, std::string>> Dictionary::findByPrefix(const std::string& prefix) const
  {
    std::vector<std::pair<std::string, std::string>> result;
    struct PrefixMatcher
    {
      const std::string& prefix;
      bool operator()(const std::pair<std::string, std::string>& p) const
      {
        return p.first.find(prefix) == 0;
      }
    } matcher{ prefix };

    std::copy_if(data.begin(), data.end(), std::back_inserter(result), matcher);

    return result;
  }

  std::vector<std::pair<std::string, std::string>> Dictionary::searchByPattern(const std::string& pattern) const
  {
    std::vector<std::pair<std::string, std::string>> result;
    struct PatternMatcher
    {
      const std::string& pattern;
      bool operator()(const std::pair<std::string, std::string>& p) const
      {
        return CommandProcessor::isPatternMatch(p.first, pattern);
      }
    }
    matcher{ pattern };

    std::copy_if(data.begin(), data.end(), std::back_inserter(result), matcher);

    return result;
  }


  CommandProcessor::CommandProcessor() : nextDictId(0) {}

  bool CommandProcessor::isValidDict(int dictId) const
  {
    return dictionaries.find(dictId) != dictionaries.end();
  }

  bool CommandProcessor::wordExistsInDict(int dictId, const std::string& word) const
  {
    auto it = dictionaries.find(dictId);
    return it != dictionaries.end() && it->second.contains(word);
  }

  bool CommandProcessor::loadDictionaryFromFile(int dictId, const std::string& filename)
  {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    std::string line;
    while (std::getline(file, line)) {
      size_t delimiterPos = line.find(" - ");
      if (delimiterPos == std::string::npos) continue;

      std::string word = line.substr(0, delimiterPos);
      std::string translation = line.substr(delimiterPos + 3);
      dictionaries[dictId].insert(word, translation);
    }
    return true;
  }

  bool CommandProcessor::saveDictionaryToFile(int dictId, const std::string& filename) const
  {
    auto it = dictionaries.find(dictId);
    if (it == dictionaries.end()) return false;

    std::ofstream file(filename);
    if (!file.is_open()) return false;

    auto words = it->second.getAllWords();

    struct WordFormatter
    {
      std::string operator()(const std::pair<std::string, std::string>& p) const
      {
        return p.first + " - " + p.second;
      }
    } formatter;

    std::vector<std::string> formattedWords;
    formattedWords.reserve(words.size());
    std::transform(words.begin(), words.end(),
    std::back_inserter(formattedWords),
    formatter);

    std::copy(formattedWords.begin(), formattedWords.end(),
    std::ostream_iterator<std::string>(file, "\n"));

    return true;
  }

  std::vector<std::string> CommandProcessor::splitCommand(const std::string& command) const
  {
    std::vector<std::string> tokens;
    if (command.empty()) return tokens;

    struct Splitter
    {
      const std::string& cmd;
      std::vector<std::string>& toks;
      size_t pos = 0;

      void skipWhitespace()
      {
        if (pos < cmd.size() && std::isspace(cmd[pos]))
        {
          ++pos;
          skipWhitespace();
        }
      }

      void addToken()
      {
        size_t start = pos;
        advanceToTokenEnd();
        if (start != pos)
        {
          toks.push_back(cmd.substr(start, pos - start));
        }
        skipWhitespace();
        if (pos < cmd.size())
        {
          addToken();
        }
      }

      void advanceToTokenEnd()
      {
        if (pos < cmd.size() && !std::isspace(cmd[pos]))
        {
          ++pos;
          advanceToTokenEnd();
        }
      }
    } splitter{ command, tokens };

    splitter.skipWhitespace();
    splitter.addToken();

    return tokens;
  }

  bool CommandProcessor::isPatternMatch(const std::string& word, const std::string& pattern)
  {
    struct Matcher
    {
      const std::string& word;
      const std::string& pattern;
      size_t wordPos;
      size_t patternPos;

      bool match()
      {
        if (patternPos >= pattern.size() || wordPos >= word.size())
        {
          return (patternPos == pattern.size() || pattern[patternPos] == '*') && wordPos == word.size();
        }
        if (pattern[patternPos] == '*')
        {
          return matchAsterisk();
        }
        else if (pattern[patternPos] == word[wordPos])
        {
          patternPos++;
          wordPos++;
          return match();
        }
        else
        {
          return false;
        }
      }

      bool matchAsterisk()
      {
        patternPos++;
        if (patternPos >= pattern.size()) return true;
        struct StarMatcher
        {
          Matcher& parent;
          bool operator()()
          {
            if (parent.wordPos >= parent.word.size()) return false;
            if (parent.word[parent.wordPos] == parent.pattern[parent.patternPos])
            {
              size_t savedWordPos = parent.wordPos;
              size_t savedPatternPos = parent.patternPos;
              parent.wordPos++;
              parent.patternPos++;
              if (parent.match()) return true;
              parent.wordPos = savedWordPos;
              parent.patternPos = savedPatternPos;
            }
            parent.wordPos++;
            return (*this)();
          }
        } starMatcher{ *this };
        return starMatcher();
      }
    };

    Matcher matcher{ word, pattern, 0, 0 };
    return matcher.match();
  }

  std::string CommandProcessor::processCommand(const std::string& command)
  {
    std::vector<std::string> tokens = splitCommand(command);
    if (tokens.empty()) return "<INVALID COMMAND>";

    const std::string& cmd = tokens[0];

    if (cmd == "download" && tokens.size() == 3)
    {
      int dictId = std::stoi(tokens[1]);
      if (!isValidDict(dictId)) return "<INVALID DICT>";
      if (!loadDictionaryFromFile(dictId, tokens[2])) return "<INVALID COMMAND>";
      return "OK";
    }
    else if (cmd == "save" && tokens.size() == 3)
    {
      int dictId = std::stoi(tokens[1]);
      if (!isValidDict(dictId)) return "<INVALID DICT>";
      if (!saveDictionaryToFile(dictId, tokens[2])) return "<INVALID COMMAND>";
      return "OK";
    }
    else if (cmd == "add" && tokens.size() >= 4)
    {
      int dictId = std::stoi(tokens[1]);
      if (!isValidDict(dictId)) return "<INVALID DICT>";

      std::string word = tokens[2];
      std::string translation;

      struct TranslationHandler
      {
        const std::vector<std::string>& tokens;
        std::string& translation;

        struct IndexProcessor
        {
          TranslationHandler& handler;
          void operator()(size_t i) const
          {
            if (i > 3) handler.translation += " ";
            handler.translation += handler.tokens[i];
          }
        } processor{ *this };
      } handler{ tokens, translation };

      std::vector<size_t> indices(tokens.size() - 3);
      std::iota(indices.begin(), indices.end(), 3);
      std::transform(indices.begin(), indices.end(), indices.begin(), handler.processor);

      if (wordExistsInDict(dictId, word)) return "<INVALID COMMAND>";
      dictionaries[dictId].insert(word, translation);
      return "OK";
    }
    else if (cmd == "remove" && tokens.size() == 3) {
      int dictId = std::stoi(tokens[1]);
      if (!isValidDict(dictId)) return "<INVALID DICT>";
      if (!wordExistsInDict(dictId, tokens[2])) return "<INVALID COMMAND>";
      dictionaries[dictId].remove(tokens[2]);
      return "OK";
    }
    else if (cmd == "translate" && tokens.size() == 3) {
      int dictId = std::stoi(tokens[1]);
      if (!isValidDict(dictId)) return "<INVALID DICT>";

      std::string translation = dictionaries[dictId].translate(tokens[2]);
      if (translation.empty()) return "<NO TRANSLATION>";
      return translation;
    }
    else if (cmd == "find" && tokens.size() == 3)
    {
      int dictId = std::stoi(tokens[1]);
      if (!isValidDict(dictId)) return "<INVALID DICT>";

      auto matches = dictionaries[dictId].findByPrefix(tokens[2]);
      if (matches.empty()) return "<NO MATCHES>";

      struct ResultBuilder
      {
        std::string result;
        void operator()(const std::pair<std::string, std::string>& p)
        {
          if (!result.empty()) result += "\n";
          result += p.first + " - " + p.second;
        }
      } builder;

      struct MatchProcessor
      {
        ResultBuilder& builder;
        std::pair<std::string, std::string> operator()(const std::pair<std::string, std::string>& p) const
        {
          builder(p);
          return p;
        }
      } processor{ builder };

      std::vector<std::pair<std::string, std::string>> temp(matches);
      std::transform(temp.begin(), temp.end(), temp.begin(), processor);

      return builder.result;
    }
    else if (cmd == "search" && tokens.size() == 3)
    {
      int dictId = std::stoi(tokens[1]);
      if (!isValidDict(dictId)) return "<INVALID DICT>";

      auto matches = dictionaries[dictId].searchByPattern(tokens[2]);
      if (matches.empty()) return "<NO MATCHES>";

      struct PairConverter
      {
        std::string operator()(const std::pair<std::string, std::string>& p) const
        {
            return p.first + " - " + p.second;
        }
      } converter;

      std::vector<std::string> formattedStrings;
      formattedStrings.reserve(matches.size());
      std::transform(matches.begin(), matches.end(), std::back_inserter(formattedStrings), converter);

      struct ResultHandler
      {
        std::string result;
        void operator()(const std::string& str)
        {
          if (!result.empty()) result += "\n";
          result += str;
        }
      } handler;

      struct StringProcessor
      {
        ResultHandler& handler;
        std::string operator()(const std::string& str) const
        {
          handler(str);
          return str;
        }
      } processor{ handler };

      std::vector<std::string> temp(formattedStrings);
      std::transform(temp.begin(), temp.end(), temp.begin(), processor);

      return handler.result;
    }
    else if (cmd == "count" && tokens.size() == 2)
    {
      int dictId = std::stoi(tokens[1]);
      if (!isValidDict(dictId)) return "<INVALID DICT>";
      return "Words: " + std::to_string(dictionaries[dictId].size());
    }
    else if (cmd == "list" && tokens.size() == 2)
    {
      int dictId = std::stoi(tokens[1]);
      if (!isValidDict(dictId)) return "<INVALID DICT>";

      auto words = dictionaries[dictId].getAllWords();
      if (words.empty()) return "<EMPTY DICTIONARY>";

      struct PairToString
      {
        std::string operator()(const std::pair<std::string, std::string>& p) const
        {
          return p.first + " - " + p.second;
        }
      } transformer;

      std::vector<std::string> formattedWords;
      formattedWords.reserve(words.size());
      std::transform(words.begin(), words.end(), std::back_inserter(formattedWords), transformer);

      struct Concatenator
      {
        std::string result;
        void operator()(const std::string& str)
        {
          if (!result.empty()) result += "\n";
          result += str;
        }
      } concatenator;

      struct StringProcessor
      {
        Concatenator& concat;
        std::string operator()(const std::string& str) const
        {
          concat(str);
          return str;
        }
      } processor{ concatenator };

      std::vector<std::string> temp(formattedWords);
      std::transform(temp.begin(), temp.end(), temp.begin(), processor);

      return concatenator.result;
    }
    else if (cmd == "clear" && tokens.size() == 2)
    {
      int dictId = std::stoi(tokens[1]);
      if (!isValidDict(dictId)) return "<INVALID DICT>";
      dictionaries[dictId].clear();
      return "Словарь " + tokens[1] + " очищен";
    }
    else if (cmd == "create_dict" && tokens.size() == 1)
    {
      int newId = nextDictId++;
      dictionaries[newId] = Dictionary();
      return "Dictionary " + std::to_string(newId) + " created";
    }
    else if (cmd == "list_dicts" && tokens.size() == 1)
    {
      if (dictionaries.empty()) return "No dictionaries";

      struct DictInfo
      {
        std::string operator()(const std::pair<int, Dictionary>& p) const
        {
          return std::to_string(p.first) + ": " + std::to_string(p.second.size()) + " words";
        }
      } dictInfo;

      std::vector<std::string> dictInfos;
      dictInfos.reserve(dictionaries.size());
      std::transform(dictionaries.begin(), dictionaries.end(), std::back_inserter(dictInfos), dictInfo);

      struct Concatenator
      {
        std::string result;
        void operator()(const std::string& str)
        {
          if (!result.empty()) result += "\n";
          result += str;
        }
      } concatenator;

      struct StringProcessor
      {
        Concatenator& concat;
        std::string operator()(const std::string& str) const
        {
          concat(str);
          return str;
        }
      } processor{ concatenator };

      std::vector<std::string> temp(dictInfos);
      std::transform(temp.begin(), temp.end(), temp.begin(), processor);

      return concatenator.result;
    }
    else if (cmd == "copy" && tokens.size() == 4)
    {
      int fromDict = std::stoi(tokens[1]);
      int toDict = std::stoi(tokens[2]);
      if (!isValidDict(fromDict) || !isValidDict(toDict)) return "<INVALID DICT>";

      std::string word = tokens[3];
      if (!wordExistsInDict(fromDict, word)) return "<NO TRANSLATION>";
      if (wordExistsInDict(toDict, word)) return "<INVALID COMMAND>";

      std::string translation = dictionaries[fromDict].translate(word);
      dictionaries[toDict].insert(word, translation);
      return "OK";
    }
    else if (cmd == "merge" && tokens.size() == 4)
    {
      int newDict = std::stoi(tokens[1]);
      int dict1 = std::stoi(tokens[2]);
      int dict2 = std::stoi(tokens[3]);
      if (!isValidDict(dict1) || !isValidDict(dict2)) return "<INVALID DICT>";

      dictionaries[newDict] = Dictionary();
      auto words1 = dictionaries[dict1].getAllWords();
      auto words2 = dictionaries[dict2].getAllWords();

      std::map<std::string, std::string> merged;
      std::set<std::string> conflicts;

      struct MergeHandler
      {
        std::map<std::string, std::string>& merged;
        std::set<std::string>& conflicts;

        struct WordProcessor
        {
          MergeHandler& handler;
          void operator()(const std::pair<std::string, std::string>& p) const
          {
            auto it = handler.merged.find(p.first);
            if (it != handler.merged.end() && it->second != p.second)
            {
              handler.conflicts.insert(p.first);
            }
            else
            {
              handler.merged[p.first] = p.second;
            }
          }
        } processor{ *this };
      } merger{ merged, conflicts };

      std::transform(words1.begin(), words1.end(), words1.begin(), merger.processor);
      std::transform(words2.begin(), words2.end(), words2.begin(), merger.processor);

      if (!conflicts.empty()) return "<CONFLICT>";

      struct DictInserter
      {
        Dictionary& target;

        struct InsertProcessor
        {
          DictInserter& inserter;
          void operator()(const std::pair<std::string, std::string>& p) const
          {
            inserter.target.insert(p.first, p.second);
          }
        } processor{ *this };
      } inserter{ dictionaries[newDict] };

      std::transform(merged.begin(), merged.end(), merged.begin(), inserter.processor);

      return "OK";
    }
    else if (cmd == "diff" && tokens.size() == 4)
    {
      int newDict = std::stoi(tokens[1]);
      int dict1 = std::stoi(tokens[2]);
      int dict2 = std::stoi(tokens[3]);
      if (!isValidDict(dict1) || !isValidDict(dict2)) return "<INVALID DICT>";

      dictionaries[newDict] = Dictionary();
      auto words1 = dictionaries[dict1].getAllWords();
      auto words2 = dictionaries[dict2].getAllWords();
      std::set<std::string> words2Set;
      bool hasDiff = false;

      struct DiffHandler
      {
        std::set<std::string>& words2Set;
        Dictionary& newDict;
        bool& hasDiff;

        struct SetInserter
        {
          DiffHandler& handler;
          void operator()(const std::pair<std::string, std::string>& p) const
          {
            handler.words2Set.insert(p.first);
          }
        } setInserter{ *this };

        struct DiffChecker
        {
          DiffHandler& handler;
          void operator()(const std::pair<std::string, std::string>& p) const
          {
            if (handler.words2Set.find(p.first) == handler.words2Set.end())
            {
              handler.newDict.insert(p.first, p.second);
              handler.hasDiff = true;
            }
          }
        } diffChecker{ *this };
      } handler{ words2Set, dictionaries[newDict], hasDiff };

      std::transform(words2.begin(), words2.end(), words2.begin(), handler.setInserter);
      std::transform(words1.begin(), words1.end(), words1.begin(), handler.diffChecker);

      if (!hasDiff) return "<EMPTY DIFF>";
      return "OK";
    }
    else if (cmd == "intersect" && tokens.size() == 4)
    {
      int newDict = std::stoi(tokens[1]);
      int dict1 = std::stoi(tokens[2]);
      int dict2 = std::stoi(tokens[3]);
      if (!isValidDict(dict1) || !isValidDict(dict2)) return "<INVALID DICT>";

      dictionaries[newDict] = Dictionary();
      auto words1 = dictionaries[dict1].getAllWords();
      auto words2 = dictionaries[dict2].getAllWords();
      std::set<std::string> words2Set;
      bool hasIntersect = false;

      struct IntersectHandler
      {
        std::set<std::string>& words2Set;
        Dictionary& resultDict;
        bool& intersectFound;

        struct SetBuilder
        {
          IntersectHandler& handler;
          void operator()(const std::pair<std::string, std::string>& p) const
          {
            handler.words2Set.insert(p.first);
          }
        } setBuilder{ *this };

        struct IntersectProcessor
        {
          IntersectHandler& handler;
          void operator()(const std::pair<std::string, std::string>& p) const
          {
            if (handler.words2Set.find(p.first) != handler.words2Set.end())
            {
              handler.resultDict.insert(p.first, p.second);
              handler.intersectFound = true;
            }
          }
        } intersectProcessor{ *this };
      } handler{ words2Set, dictionaries[newDict], hasIntersect };

      std::transform(words2.begin(), words2.end(), words2.begin(), handler.setBuilder);

      std::transform(words1.begin(), words1.end(), words1.begin(), handler.intersectProcessor);

      if (!hasIntersect) return "<EMPTY INTERSECT>";
      return "OK";
    }
    else if (cmd == "global_search" && tokens.size() == 2)
    {
      std::string pattern = tokens[1];
      std::string result;

      struct GlobalSearchProcessor
      {
        const std::string& pattern;
        std::string& result;

        struct DictProcessor
        {
          GlobalSearchProcessor& processor;
          void operator()(const std::pair<int, Dictionary>& dict) const
          {
            auto matches = dict.second.searchByPattern(processor.pattern);

            struct MatchFormatter
            {
              int dictId;
              std::string& result;
              void operator()(const std::pair<std::string, std::string>& p) const
              {
                result += "[" + std::to_string(dictId) + "] " + p.first + " - " + p.second + "\n";
              }
            } formatter{ dict.first, processor.result };

            std::transform(matches.begin(), matches.end(), matches.begin(), formatter);
          }
        } dictProcessor{ *this };
      } processor{ pattern, result };

      std::transform(dictionaries.begin(), dictionaries.end(), dictionaries.begin(), processor.dictProcessor);

      if (result.empty()) return "<NO MATCHES>";
      result.pop_back();
      return result;
    }
    else if (cmd == "global_count" && tokens.size() == 1)
    {
      struct GlobalCounter
      {
        size_t total = 0;
        std::string result;

        struct Counter
        {
          GlobalCounter& handler;
          void operator()(const std::pair<int, Dictionary>& dict)
          {
            handler.total += dict.second.size();
          }
        } counter{ *this };

        struct Printer
        {
          GlobalCounter& handler;
          void operator()(const std::pair<int, Dictionary>& dict)
          {
            handler.result += std::to_string(dict.first) + ": " + std::to_string(dict.second.size()) + "\n";
          }
        } printer{ *this };
      } processor;

      std::transform(dictionaries.begin(), dictionaries.end(), dictionaries.begin(), processor.counter);

      processor.result = "Total words: " + std::to_string(processor.total) + "\nPer dictionary:\n";

      std::transform(dictionaries.begin(), dictionaries.end(), dictionaries.begin(), processor.printer);

      if (!processor.result.empty()) processor.result.pop_back();
      return processor.result;
    }
    else if (cmd == "help" && tokens.size() == 1)
    {
      return help();
    }

    return "<INVALID COMMAND>";
  }

  std::string CommandProcessor::help() const
  {
    return "Доступные команды:\n"
    "download <dict_id> <filename> - загрузить словарь из файла\n"
    "save <dict_id> <filename> - сохранить словарь в файл\n"
    "add <dict_id> <word> <translation> - добавить слово и перевод\n"
    "remove <dict_id> <word> - удалить слово из словаря\n"
    "translate <dict_id> <word> - получить перевод слова\n"
    "find <dict_id> <prefix> - найти слова по префиксу\n"
    "search <dict_id> <pattern> - поиск по шаблону (* - любая последовательность)\n"
    "count <dict_id> - показать количество слов в словаре\n"
    "list <dict_id> - вывести все слова из словаря\n"
    "clear <dict_id> - очистить словарь\n"
    "create_dict - создать новый словарь\n"
    "list_dicts - вывести список всех словарей\n"
    "copy <from_dict> <to_dict> <word> - скопировать слово между словарями\n"
    "merge <new_dict> <dict1> <dict2> - объединить два словаря\n"
    "diff <new_dict> <dict1> <dict2> - показать различия между словарями\n"
    "intersect <new_dict> <dict1> <dict2> - показать общие слова\n"
    "global_search <pattern> - поиск по всем словарям\n"
    "global_count - общая статистика по всем словарям\n"
    "help - вывести справку";
  }
}
