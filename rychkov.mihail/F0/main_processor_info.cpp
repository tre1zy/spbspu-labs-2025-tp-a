#include "main_processor.hpp"

#include <fstream>
#include <algorithm>
#include <iterator>

bool rychkov::MainProcessor::files(ParserContext& context)
{
  if (!eol(context.in))
  {
    return false;
  }
  for (const decltype(parsed_)::value_type& i: parsed_)
  {
    context.out << i.first << '\n';
  }
  return true;
}
bool rychkov::MainProcessor::tree(ParserContext& context)
{
  std::string filename;
  if ((last_stage_ != CPARSER) || !(context.in >> filename) || !eol(context.in))
  {
    return false;
  }
  parsed_.at(filename).preproc.next->next->print(context.out);
  return true;
}
void rychkov::MainProcessor::help(std::ostream& out)
{
  std::ifstream file(help_file);
  if (!file)
  {
    out << "failed to open help file - \"" << help_file << "\"\n";
    return;
  }
  out << file.rdbuf();
}
