#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <iosfwd>
#include <functional>
#include "index.hpp"

namespace krylov
{
  class CommandProcessor
  {
  public:
    CommandProcessor(std::ostream& output);
    void execute(const std::string& line);
  private:
    IndexStorage storage_;
    std::ostream& out_;
    std::map< std::string, IndexDocument > indexes_;
    std::map< std::string, std::function< void(const std::vector< std::string >&) > > commands;
    void initializeCommands();
    void createIndex(const std::string& indexName, const std::string& filename);
    void findWord(const std::string& indexName, const std::string& word);
    void printIndex(const std::string& indexName);
    void deleteIndex(const std::string& indexName);
    void listIndexes();
    void mergeIndexes(const std::string& index1, const std::string& index2, const std::string& newIndex);
    void zipTexts(const std::string& index1, const std::string& index2, const std::string& newIndex);

    void createIndexCmd(const std::vector< std::string >& args);
    void printIndexCmd(const std::vector< std::string >& args);
    void findWordCmd(const std::vector< std::string >& args);
    void mergeIndexesCmd(const std::vector< std::string >& args);
    void zipTextsCmd(const std::vector< std::string >& args);
    void listIndexesCmd(const std::vector< std::string >& args);
    void deleteIndexCmd(const std::vector< std::string >& args);
  };
}

#endif
