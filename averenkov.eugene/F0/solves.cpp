#include <numeric>
#include "solves.hpp"
#include "commands.hpp"

void averenkov::CombinationBuilder::operator()(int pos) const
{
  if (MaskChecker(pos)(pos))
  {
    current.push_back(items[pos]);
  }
}

averenkov::CombinationEvaluator::CombinationEvaluator(int cap, std::vector<const Item*>& best, int& maxVal, int& bestWgt):
  capacity(cap),
  bestCombination(best),
  maxValue(maxVal),
  bestWeight(bestWgt)
{}

void averenkov::CombinationEvaluator::operator()(const std::vector<const Item*>& combination)
{
  int totalWeight = calculateTotalWeight(combination);
  int totalValue = calculateTotalValue(combination);

  if (totalWeight <= capacity && (totalValue > maxValue || (totalValue == maxValue && totalWeight < bestWeight)))
  {
    bestCombination = combination;
    maxValue = totalValue;
    bestWeight = totalWeight;
  }
}

void averenkov::DPTableInitializer::operator()(std::vector<int>& row) const
{
  row.resize(capacity + 1);
  std::fill(row.begin(), row.end(), 0);
}

void averenkov::DPRowFiller::operator()()
{
  if (current_weight > 0)
  {
    if (items[current_index-1]->getWeight() <= current_weight)
    {
      auto te = dp[current_index-1][current_weight];
      auto tu = dp[current_index-1][current_weight - items[current_index-1]->getWeight()];
      dp[current_index][current_weight] = std::max( te, tu + items[current_index-1]->getValue());
    }
    else
    {
      dp[current_index][current_weight] = dp[current_index-1][current_weight];
    }
    DPRowFiller next{items, dp, current_index, current_weight - 1};
    next();
  }
}

void averenkov::DPRowProcessor::operator()()
{
  if (current_index <= items.size())
  {
    int last_weight = dp[current_index].empty() ? 0 : dp[current_index].size() - 1;
    DPRowFiller filler{items, dp, current_index, last_weight};
    filler();
    DPRowProcessor next{items, dp, current_index + 1};
    next();
  }
}

void averenkov::SolutionBuilder::operator()()
{
  if (current_index > 0 && remaining_weight > 0)
  {
    if (dp[current_index][remaining_weight] != dp[current_index-1][remaining_weight])
    {
      result.push_back(items[current_index-1]);
      remaining_weight -= items[current_index-1]->getWeight();
    }
    SolutionBuilder next{items, dp, result, remaining_weight, current_index - 1};
    next();
  }
}

void averenkov::generateCombinations(const std::vector<const Item*>& items,
      std::vector<std::vector<const Item*>>& allCombinations,
      std::vector<const Item*> currentCombination,
      size_t index)
{
  if (index == items.size())
  {
    allCombinations.push_back(currentCombination);
    return;
  }
  generateCombinations(items, allCombinations, currentCombination, index + 1);

  currentCombination.push_back(items[index]);
  generateCombinations(items, allCombinations, currentCombination, index + 1);
}

int averenkov::calculateTotalWeight(const std::vector<const Item*>& combination)
{
  return std::accumulate(combination.begin(), combination.end(), 0, WeightCalculator());
}

int averenkov::calculateTotalValue(const std::vector<const Item*>& combination)
{
  return std::accumulate(combination.begin(), combination.end(), 0, ValueCalculator());
}



void averenkov::bruteforce(Base& base, const std::vector<std::string>& args)
{
  if (args.size() < 3)
  {
    throw std::invalid_argument("Not enough arguments for bruteforce command");
  }

  const std::string& sourceKitName = args[1];
  const std::string& resultKitName = args[2];

  auto kitIt = base.kits.find(sourceKitName);
  if (kitIt == base.kits.end())
  {
    throw std::invalid_argument("Source kit not found");
  }

  const Kit& sourceKit = kitIt->second;
  int capacity = base.current_knapsack.getCapacity();
  std::vector<const Item*> items = sourceKit.getItems();

  std::vector<std::vector<const Item*>> allCombinations;
  generateCombinations(items, allCombinations, {}, 0);

  std::vector<const Item*> bestCombination;
  int maxValue = 0;
  int bestWeight = 0;
  CombinationEvaluator evaluator(capacity, bestCombination, maxValue, bestWeight);
  std::for_each(allCombinations.begin(), allCombinations.end(), evaluator);

  if (base.kits.find(resultKitName) != base.kits.end())
  {
    throw std::invalid_argument("Kit already exists");
  }
  auto emplaceResult = base.kits.emplace(resultKitName, Kit(resultKitName));
  Kit& resultKit = emplaceResult.first->second;

  ItemAdder adder(resultKit);
  std::for_each(bestCombination.begin(), bestCombination.end(), adder);
}

void averenkov::dynamicProgrammingSolve(Base& base, const std::vector<std::string>& args)
{
  if (args.size() < 3)
  {
    throw std::invalid_argument("Not enough arguments for dp command");
  }

  const std::string& sourceKitName = args[1];
  const std::string& resultKitName = args[2];

  auto kitIt = base.kits.find(sourceKitName);
  if (kitIt == base.kits.end())
  {
    throw std::invalid_argument("Source kit not found");
  }

  if (base.kits.find(resultKitName) != base.kits.end())
  {
    throw std::invalid_argument("Result kit already exists");
  }

  const Kit& sourceKit = kitIt->second;
  int capacity = base.current_knapsack.getCapacity();
  const std::vector<const Item*>& items = sourceKit.getItems();

  std::vector<std::vector<int>> dp(items.size() + 1);
  DPTableInitializer init{dp, capacity};
  std::for_each(dp.begin(), dp.end(), init);

  DPRowProcessor processor{items, dp, 1};
  processor();

  std::vector<const Item*> selectedItems;
  int remaining_weight = capacity;
  SolutionBuilder builder{items, dp, selectedItems, remaining_weight, items.size()};
  builder();

  Kit& resultKit = base.kits.emplace(resultKitName, Kit(resultKitName)).first->second;
  ItemAdder adder(resultKit);
  std::for_each(selectedItems.begin(), selectedItems.end(), adder);
}
