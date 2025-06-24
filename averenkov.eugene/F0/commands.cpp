#include <iostream>
#include <numeric>
#include "commands.hpp"

bool averenkov::ItemFinder::operator()(const Item& item) const
{
  return item.getName() == name;
}

bool averenkov::ItemFinderPtr::operator()(const Item* item) const
{
  return item->getName() == name;
}

bool averenkov::KitFinder::operator()(const std::pair<const std::string, Kit>& kit) const
{
  return kit.first == name;
}

void averenkov::KitItemRemover::operator()(std::pair<const std::string, Kit>& kit) const
{
  kit.second.removeItem(name);
}

const averenkov::Item* averenkov::ItemCopier::operator()(const averenkov::Item* item) const
{
  return item;
}

averenkov::MaskChecker::MaskChecker(int m):
  mask(m)
{}

bool averenkov::MaskChecker::operator()(int pos) const
{
  return mask & (1 << pos);
}

int averenkov::WeightCalculator::operator()(int sum, const Item* item)
{
  return sum + item->getWeight();
}

int averenkov::ValueCalculator::operator()(int sum, const Item* item)
{
  return sum + item->getValue();
}

averenkov::ItemAdder::ItemAdder(Kit& k):
  kit(k)
{}

void averenkov::ItemAdder::operator()(const Item* item)
{
  kit.addItem(item);
}

void averenkov::ItemPrinter::operator()(const Item& item) const
{
  std::cout << "Item: " << item.getName();
  std::cout << ", Weight: " << item.getWeight();
  std::cout << ", Value: " << item.getValue() << "\n";
}

void averenkov::KitItemPrinter::operator()(const Item* item) const
{
  std::cout << "    - " << item->getName() << "\n";
}

void averenkov::KitPrinter::operator()(const std::pair<const std::string, Kit>& kit_pair) const
{
  std::cout << "Kit: " << kit_pair.first << "\n";
  std::cout << "  Items:\n";
  auto its = kit_pair.second.getItems();
  std::for_each(its.begin(), its.end(), KitItemPrinter());
}

void averenkov::KnapsackPrinter::operator()(const std::pair<const std::string, Knapsack>& knapsack_pair) const
{
  std::cout << "Knapsack: " << knapsack_pair.first;
  std::cout << ", Capacity: " << knapsack_pair.second.getCapacity() << "\n";
}

void averenkov::CombinationBuilder::operator()(int pos) const
{
  if (MaskChecker(pos)(pos))
  {
    current.push_back(items[pos]);
  }
}


void averenkov::addItem(Base& base, const std::vector<std::string>& args)
{
  if (args.size() != 4)
  {
    throw std::invalid_argument("Invalid arguments count for add");
  }

  if (std::any_of(base.items.begin(), base.items.end(), ItemFinder{ args[1] }))
  {
    throw std::invalid_argument("Item already exists");
  }

  base.items.emplace_back(args[1], std::stoi(args[2]), std::stoi(args[3]));
}

// 2. remove <name>
void averenkov::removeItem(Base& base, const std::vector<std::string>& args)
{
  if (args.size() != 2)
  {
    throw std::invalid_argument("Invalid arguments count for remove");
  }

  auto it = std::find_if(base.items.begin(), base.items.end(), ItemFinder{ args[1] });
  if (it == base.items.end())
  {
    throw std::invalid_argument("Item not found");
  }

  base.items.erase(it);
}

// 3. edit <name> <new_weight> <new_value>
void averenkov::editItem(Base& base, const std::vector<std::string>& args)
{
  if (args.size() != 4)
  {
    throw std::invalid_argument("Invalid arguments count for edit");
  }

  auto it = std::find_if(base.items.begin(), base.items.end(), ItemFinder{ args[1] });
  if (it == base.items.end())
  {
    throw std::invalid_argument("Item not found");
  }

  it->setWeight(std::stoi(args[2]));
  it->setValue(std::stoi(args[3]));
}

// 4. add_kit <kit_name>
void averenkov::addKit(Base& base, const std::vector<std::string>& args)
{
  if (args.size() != 2)
  {
    throw std::invalid_argument("Invalid arguments count for add_kit");
  }

  if (base.kits.find(args[1]) != base.kits.end())
  {
    throw std::invalid_argument("Kit already exists");
  }

  base.kits.emplace(args[1], Kit(args[1]));
}

// 5. remove_kit <kit_name>
void averenkov::removeKit(Base& base, const std::vector<std::string>& args)
{
  if (args.size() != 2)
  {
    throw std::invalid_argument("Invalid arguments count for remove_kit");
  }

  if (base.kits.erase(args[1]) == 0)
  {
    throw std::invalid_argument("Kit not found");
  }
}

// 6. add_to_kit <kit_name> <item_name>
void averenkov::addToKit(Base& base, const std::vector<std::string>& args)
{
  if (args.size() != 3)
  {
    throw std::invalid_argument("Invalid arguments count for add_to_kit");
  }

  auto kit_it = base.kits.find(args[1]);
  if (kit_it == base.kits.end())
  {
    throw std::invalid_argument("Kit not found");
  }


  auto item_it = std::find_if(base.items.begin(), base.items.end(), ItemFinder{ args[2] });
  if (item_it == base.items.end())
  {
    throw std::invalid_argument("Item not found");
  }

  const auto& items_in_kit = kit_it->second.getItems();
  Item* item_ptr = std::addressof(*item_it);

  if (std::find(items_in_kit.begin(), items_in_kit.end(), item_ptr) != items_in_kit.end())
  {
    throw std::invalid_argument("Item already in kit");
  }

  kit_it->second.addItem(std::addressof(*item_it));
}

// 7. remove_from_kit <kit_name> <item_name>
void averenkov::removeFromKit(Base& base, const std::vector<std::string>& args)
{
  if (args.size() != 3)
  {
    throw std::invalid_argument("Invalid arguments count for remove_from_kit");
  }

  auto kit_it = base.kits.find(args[1]);
  if (kit_it == base.kits.end())
  {
    throw std::invalid_argument("Kit not found");
  }

  const auto& items_in_kit = kit_it->second.getItems();
  auto it = std::find_if(items_in_kit.begin(), items_in_kit.end(), ItemFinderPtr{ args[2] });

  if (it == items_in_kit.end())
  {
    throw std::invalid_argument("Item not found in kit");
  }

  kit_it->second.removeItem(args[2]);
}

// 8. add_knapsack <knapsack_name> <capacity>
void averenkov::addKnapsack(Base& base, const std::vector<std::string>& args)
{
  if (args.size() != 3)
  {
    throw std::invalid_argument("Invalid arguments count for add_knapsack");
  }

  if (base.knapsacks.find(args[1]) != base.knapsacks.end())
  {
    throw std::invalid_argument("Knapsack already exists");
  }

  base.knapsacks.emplace(args[1], Knapsack(std::stoi(args[2])));
}

// 9. set_knapsack <knapsack_name>
void averenkov::setKnapsack(Base& base, const std::vector<std::string>& args)
{
  if (args.size() != 2)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  if (base.knapsacks.find(args[1]) == base.knapsacks.end())
  {
    throw std::invalid_argument("<NOT FOUND>");
  }

  base.current_knapsack = base.knapsacks.find(args[1])->second;
}

// 11. stats
void averenkov::showStats(const Base& base, const std::vector<std::string>& args)
{
  if (args.size() != 1)
  {
    throw std::invalid_argument("stats command takes no arguments");
  }
    std::cout << "=== Items ===\n";
    std::for_each(base.items.begin(), base.items.end(), ItemPrinter());
    std::cout << "\n=== Kits ===\n";
    std::for_each(base.kits.begin(), base.kits.end(), KitPrinter());
    std::cout << "\n=== Knapsacks ===\n";
    std::for_each(base.knapsacks.begin(), base.knapsacks.end(), KnapsackPrinter());
    std::cout << "\n=== Current Knapsack ===\n";
    std::cout << "Capacity: " << base.current_knapsack.getCapacity() << "\n";
}

// 12. reset
void averenkov::reset(Base& base, const std::vector<std::string>& args)
{
  if (args.size() != 1)
  {
    throw std::invalid_argument("reset command takes no arguments");
  }

  base.kits.clear();
  base.knapsacks.clear();
}

/*void averenkov::bruteforce(averenkov::Base& base, const std::vector<std::string>& args)
{
  if (args.size() != 3)
  {
    throw std::invalid_argument("Usage: bruteforce <source_kit> <result_kit>");
  }

  const std::string& source_kit = args[1];
  const std::string& result_kit = args[2];
  if (base.kits.find(result_kit) != base.kits.end())
  {
    throw std::invalid_argument("Result kit exist");
  }
  const auto kit_it = base.kits.find(source_kit);
  if (kit_it == base.kits.end())
  {
    throw std::invalid_argument("Source kit not found");
  }

  std::vector<const Item*> items;
  const auto& source_items = kit_it->second.getItems();
  items.resize(source_items.size());
  std::transform(source_items.begin(), source_items.end(), items.begin(), ItemCopier{});

  int max_value = 0;
  std::vector<const Item*> best_combination;
  int capacity = base.current_knapsack.getCapacity();

  generateCombinations(items, 0, max_value, best_combination, capacity);

  Kit result(result_kit);
  std::for_each(best_combination.begin(), best_combination.end(), ItemAdder{ result });
  base.kits.emplace(result_kit, std::move(result));
}

void averenkov::generateCombinations(const std::vector<const averenkov::Item*>& items, int mask, int& max_value, std::vector<const averenkov::Item*>& best_combination, const int capacity)
{
  if (mask >= (1 << items.size()))
  {
    return;
  }

  std::vector< const averenkov::Item* > current;
  std::vector< int > indices(items.size());
  std::iota(indices.begin(), indices.end(), 0);
  std::for_each(indices.begin(), indices.end(), CombinationBuilder{ current, items });

  const int current_weight = std::accumulate(current.begin(), current.end(), 0, WeightCalculator{});
  const int current_value = std::accumulate(current.begin(), current.end(), 0, ValueCalculator{});

  if (current_weight <= capacity && current_value > max_value)
  {
    max_value = current_value;
    best_combination = current;
  }

  generateCombinations(items, mask + 1, max_value, best_combination, capacity);
}*/

