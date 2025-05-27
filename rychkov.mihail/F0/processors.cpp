#include "processors.hpp"

#include <iostream>

bool rychkov::MainProcessor::init(ParserContext& context, int argc, char** argv)
{
  return true;
}
bool rychkov::MainProcessor::help(ParserContext& context)
{
  context.out << "<[help info]>\n";
  return true;
}
