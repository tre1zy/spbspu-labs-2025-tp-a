#ifndef COMMANDS_H
#define COMMANDS_H

#include <map>
#include <set>
#include <string>
#include <vector>

namespace kharlamov
{
  class Dictionary;

  class CommandProcessor
  {
  private:
    std::map<int, Dictionary> dictionaries;
    int nextDictId;

    bool isValidDict(int dictId) const;
    bool wordExistsInDict(int dictId, const std::string& word) const;
    bool loadDictionaryFromFile(int dictId, const std::string& filename);
    bool saveDictionaryToFile(int dictId, const std::string& filename) const;
    std::vector<std::string> splitCommand(const std::string& command) const;

  public:
    CommandProcessor();
    static bool isPatternMatch(const std::string& word, const std::string& pattern);
    std::string processCommand(const std::string& command);
    std::string help() const;
  };

  class Dictionary
  {
  private:
    std::map<std::string, std::string> data;

  public:
    void insert(const std::string& key, const std::string& value);
    bool contains(const std::string& key) const;
    std::string translate(const std::string& key) const;
    bool remove(const std::string& key);
    void clear();
    size_t size() const;
    bool empty() const;
    std::vector<std::pair<std::string, std::string>> getAllWords() const;
    std::vector<std::pair<std::string, std::string>> findByPrefix(const std::string& prefix) const;
    std::vector<std::pair<std::string, std::string>> searchByPattern(const std::string& pattern) const;
  };

}

#endif // COMMANDS_H
