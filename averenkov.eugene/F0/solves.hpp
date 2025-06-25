#ifndef SOLVES_HPP
#define SOLVES_HPP
#include "commands.hpp"

namespace averenkov
{
  struct CombinationEvaluator
  {
  public:
    CombinationEvaluator(int cap, std::vector<const Item*>& best, int& maxVal, int& bestWgt);
    void operator()(const std::vector<const Item*>& combination);

  private:
    int capacity;
    std::vector<const Item*>& bestCombination;
    int& maxValue;
    int& bestWeight;
  };

  struct CombinationBuilder
  {
    std::vector< const Item* >& current;
    const std::vector< const Item* >& items;
    void operator()(int pos) const;
  };

  struct DPTableInitializer
  {
    std::vector<std::vector<int>>& dp;
    int capacity;

    void operator()(std::vector<int>& row) const;
  };

  struct DPRowFiller
  {
    const std::vector<const Item*>& items;
    std::vector<std::vector<int>>& dp;
    size_t current_index;
    int current_weight;

    void operator()();
  };

  struct DPRowProcessor
  {
    const std::vector<const Item*>& items;
    std::vector<std::vector<int>>& dp;
    size_t current_index;

    void operator()();
  };

  struct SolutionBuilder
  {
    const std::vector<const Item*>& items;
    const std::vector<std::vector<int>>& dp;
    std::vector<const Item*>& result;
    int& remaining_weight;
    size_t current_index;

    void operator()();
  };

  int calculateTotalWeight(const std::vector<const Item*>& combination);
  int calculateTotalValue(const std::vector<const Item*>& combination);
  void generateCombinations(const std::vector<const Item*>& items,
      std::vector<std::vector<const Item*>>& allCombinations,
      std::vector<const Item*> currentCombination,
      size_t index);

  void solve(Base& base, const std::vector< std::string >& args);
  void bruteforce(Base& base, const std::vector< std::string >& args);
  void dynamicProgrammingSolve(Base& base, const std::vector< std::string >& args);
  void backtrackingSolve(Base& base, const std::vector< std::string >& args);
  void branchAndBoundSolve(Base& base, const std::vector< std::string >& args);
}
#endif
