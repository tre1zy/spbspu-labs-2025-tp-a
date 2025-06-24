#include <iostream>
#include <unordered_map>
#include <functional>
#include <limits>
#include "graph.hpp"
#include "graph_operations.hpp"
#include "command.hpp"

int main(int argc, char* argv[]) {
  using namespace klimova;
  GraphManager graphs;

  if (argc > 1) {
    std::string option(argv[1]);
    if (option == "--help") {
      showHelp(std::cout);
    }
    else if (option == "--check" && argc == 3) {
      std::string filename(argv[2]);
      if (checkGraphFile(filename)) {
        std::cout << "[OK] Graph file is valid.\n";
      } else {
        std::cout << "[ERROR] Graph file is invalid.\n";
      }
    }
    else if (argc == 2) {
      try {
        loadGraphFromFile(graphs, argv[1]);
        std::cout << "[OK] Graphs loaded from '" << argv[1] << "'.\n";
      } catch (const std::exception& e) {
        std::cerr << "[ERROR] " << e.what() << "\n";
      }
    }
    else {
      std::cerr << "[ERROR] Invalid arguments. Use --help for usage.\n";
    }
  }

  std::cout << "\nEntering interactive mode:\n";
  auto cmdMap = createCommandHandler(graphs);
  std::string command;
  while (std::cin >> command) {
    try {
      auto it = cmdMap.find(command);
      if (it != cmdMap.end()) {
        it->second(std::cin, std::cout);
      }
      else {
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
        std::cout << "<INVALID COMMAND>\n";
      }
    }
    catch (const std::exception&) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}
