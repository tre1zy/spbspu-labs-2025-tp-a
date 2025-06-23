#include <limits>
#include "wrapper.hpp"
#include "shannonFano.hpp"

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Error in parameters\n";
    return 1;
  }
  if (std::string(argv[1]) == "--help")
  {
    duhanina::print_help(std::cout);
    return 0;
  }
  auto commands = duhanina::initialize_commands(std::cin, std::cout);
  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      commands.at(cmd)();
    }
    catch (const std::exception& e)
    {
      std::cerr << e.what() << "\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;
}
