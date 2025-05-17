#include "processors.hpp"

#include<iostream>
#include<fstream>

bool rychkov::MainProcessor::init(ParserContext& context, int argc, char** argv)
{
  if (argc != 2)
  {
    context.err << "wrong arguments count\n";
    return false;
  }
  std::ifstream file{argv[1]};
  if (!file)
  {
    context.err << "failed to open file \"" << argv[1] << "\"\n";
  }
  return true;
}
