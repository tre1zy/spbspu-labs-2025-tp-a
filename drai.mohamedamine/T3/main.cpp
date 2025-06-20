#include "polygon.hpp"

#include <iterator>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>

int main()
{
  std::vector<amine::Polygon> polygons;

  std::string all_input(
    std::istreambuf_iterator<char>(std::cin),
    std::istreambuf_iterator<char>()
  );

  std::istringstream input_stream(all_input);
  std::vector<std::string> lines{
    std::istream_iterator<std::string>(input_stream),
    std::istream_iterator<std::string>()
  };

  std::vector<std::vector<std::string>> command_blocks;
  std::vector<std::string> current;

  std::for_each(lines.begin(), lines.end(), [&](const std::string& token) {
    if (token == ";")
    {
      if (!current.empty())
      {
        command_blocks.push_back(current);
        current.clear();
      }
    }
    else
    {
      current.push_back(token);
    }
  });

  if (!current.empty())
  {
    command_blocks.push_back(current);
  }

  std::for_each(command_blocks.begin(), command_blocks.end(), [&](const std::vector<std::string>& tokens) {
    if (!tokens.empty())
    {
      amine::process_command(tokens[0], std::vector<std::string>(tokens.begin() + 1, tokens.end()), polygons);
    }
  });

  return 0;
}
