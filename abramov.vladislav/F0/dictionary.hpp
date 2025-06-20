#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP
#include <string>
#include <unordered_map>

namespace abramov
{
  struct Dictionary
  {
    Dictionary();
    Dictionary(const Dictionary &dict);
    void addWord(const std::string &word, const std::string &trans);
    void mergeDict(const Dictionary &dict);
    void deleteWord(const std::string &word);
    void diffDict(const Dictionary &dict);
    void print(std::ostream &out) const noexcept;
    Dictionary setWithDict(const Dictionary &dict) const;
    Dictionary intersectWithDict(const Dictionary &dict) const;
    Dictionary unionWithDict(const Dictionary &dict) const;
    bool empty() const noexcept;
  private:
    std::unordered_map< std::string, std::string > dict_;
  };
}
#endif
