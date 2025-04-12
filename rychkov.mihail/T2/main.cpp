#include <iostream>
#include "skip_any_of.hpp"

int main()
{
  size_t matches = 0;
  char c = '\0';
  std::cin >> rychkov::input::anyOf(&matches, "key1", "key2", "key3") >> c;
  if (!std::cin)
  {
    std::cout << "fail\n";
  }
  std::cout << matches << " '" << c << "'\n";
}
