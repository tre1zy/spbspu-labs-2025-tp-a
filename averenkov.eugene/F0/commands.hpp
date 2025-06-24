#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <stdexcept>
#include "base.hpp"

namespace averenkov
{
  struct ItemFinder
  {
    const std::string& name;
    bool operator()(const Item& item) const;
  };

  struct ItemFinderPtr
  {
    const std::string& name;
    bool operator()(const Item* item) const;
  };

  struct KitFinder
  {
    const std::string& name;
    bool operator()(const std::pair< const std::string, Kit >& kit) const;
  };

  struct KitItemRemover
  {
    const std::string& name;
    void operator()(std::pair< const std::string, Kit >& kit) const;
  };

  class ItemCopier
  {
  public:
    const Item* operator()(const Item* item) const;
  };

  struct MaskChecker
  {
    explicit MaskChecker(int m);
    bool operator()(int pos) const;
  private:
    int mask;
  };

  struct WeightCalculator
  {
    int operator()(int sum, const Item* item);
  };

  struct ValueCalculator
  {
    int operator()(int sum, const Item* item);
  };

  struct ItemAdder
  {
    explicit ItemAdder(Kit& k);
    void operator()(const Item* item);
  private:
    Kit& kit;
  };

  class ItemPrinter
  {
  public:
    void operator()(const Item& item) const;
  };

  class KitItemPrinter
  {
  public:
    void operator()(const Item* item) const;
  };

  class KitPrinter
  {
  public:
    void operator()(const std::pair<const std::string, Kit>& kit_pair) const;
  };

  class KnapsackPrinter
  {
  public:
    void operator()(const std::pair<const std::string, Knapsack>& knapsack_pair) const;
  };

  struct CombinationBuilder
  {
    std::vector< const Item* >& current;
    const std::vector< const Item* >& items;
    void operator()(int pos) const;
  };

  void generateCombinations(const std::vector< const Item* >& items, int mask, int& max_value, std::vector< const Item* >& best_combination, const int capacity);


  void addItem(Base& base, const std::vector< std::string >& args);
  void removeItem(Base& base, const std::vector< std::string >& args);
  void editItem(Base& base, const std::vector< std::string >& args);
  void addKit(Base& base, const std::vector< std::string >& args);
  void removeKit(Base& base, const std::vector< std::string >& args);
  void addToKit(Base& base, const std::vector< std::string >& args);
  void removeFromKit(Base& base, const std::vector< std::string >& args);
  void addKnapsack(Base& base, const std::vector< std::string >& args);
  void setKnapsack(Base& base, const std::vector< std::string >& args);
  void solve(Base& base, const std::vector< std::string >& args);
  void showStats(const Base& base, const std::vector< std::string >& args);
  void reset(Base& base, const std::vector< std::string >& args);
  void bruteforce(Base& base, const std::vector< std::string >& args);
  void dynamicProgrammingSolve(Base& base, const std::vector< std::string >& args);
  void backtrackingSolve(Base& base, const std::vector< std::string >& args);
  void branchAndBoundSolve(Base& base, const std::vector< std::string >& args);
  void saveToFile(const Base& base, const std::vector< std::string >& args);
  void loadFromFile(Base& base, const std::vector< std::string >& args);
}

#endif
