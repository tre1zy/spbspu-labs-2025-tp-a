#include <iostream>
#include <sstream>
#include "commands.hpp"
#include "solves.hpp"

int main()
{
  std::map<std::string, std::function< void(averenkov::Base& , const std::vector< std::string >&) > > commands;
  commands["add"] = averenkov::addItem;
  commands["remove"] = averenkov::removeItem;
  commands["edit"] = averenkov::editItem;
  commands["add_kit"] = averenkov::addKit;
  commands["remove_kit"] = averenkov::removeKit;
  commands["add_to_kit"] = averenkov::addToKit;
  commands["remove_from_kit"] = averenkov::removeFromKit;
  commands["add_knapsack"] = averenkov::addKnapsack;
  commands["set_knapsack"] = averenkov::setKnapsack;
  commands["solve"] = averenkov::solve;
  commands["stats"] = averenkov::showStats;
  commands["reset"] = averenkov::reset;
  commands["bruteforce"] = averenkov::bruteforce;
  commands["dynamic_prog"] = averenkov::dynamicProgrammingSolve;
  commands["backtracking"] = averenkov::backtrackingSolve;
  commands["branch_and_bound"] = averenkov::branchAndBoundSolve;
  commands["save"] = averenkov::saveToFile;
  commands["load"] = averenkov::loadFromFile;

  averenkov::Base base;

  std::string line;
  while(std::getline(std::cin, line))
  {
    std::vector<std::string> args;
    std::istringstream iss(line);
    std::string arg;
    while(iss >> arg)
    {
      args.push_back(arg);
    }

    if(args.empty()) continue;

    try
    {
      auto it = commands.find(args[0]);
      if(it != commands.end())
      {
        it->second(base, args);
      }
      else
      {
        std::cerr << "Unknown command: " << args[0] << std::endl;
      }
    }
    catch(const std::exception& e)
    {
      std::cerr << "Error: " << e.what() << std::endl;
    }
  }

  return 0;
}
