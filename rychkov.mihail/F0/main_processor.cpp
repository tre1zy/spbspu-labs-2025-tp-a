#include "main_processor.hpp"

#include <iostream>
#include <fstream>
#include <utility>

rychkov::Parser::map_type< rychkov::ParserContext, rychkov::MainProcessor > rychkov::MainProcessor::call_map = {
      {"tree", &rychkov::MainProcessor::tree},
      {"files", &rychkov::MainProcessor::files},
      {"reload-all", &rychkov::MainProcessor::reload},
      {"parse", &rychkov::MainProcessor::parse}
    };

rychkov::ParseCell::ParseCell(CParseContext context, Stage last_stage,
    std::vector< std::string > include_dirs):
  base_context{std::move(context)},
  preproc{(last_stage == PREPROCESSOR ? nullptr : &lex), std::move(include_dirs)},
  lex{(last_stage == PREPROCESSOR) || (last_stage == LEXER) ? nullptr : &parser},
  parser{}
{}
bool rychkov::ParseCell::parse(std::istream& in)
{
  preproc.parse(base_context, in);
  return base_context.nerrors == 0;
}

bool rychkov::MainProcessor::parse(ParserContext& context)
{
  std::string filename;
  if (!(context.in >> filename) || !eol(context.in))
  {
    return false;
  }
  std::ifstream in(filename);
  if (!in)
  {
    context.err << "failed to open file\n";
  }
  ParseCell cell = {{context.out, context.err, filename}, last_stage_, include_dirs_};
  context.out << "<--PARSE: \"" << filename << "\"-->\n";
  if (!cell.parse(in))
  {
    context.err << "failed to parse file \"" << filename << "\" - stopping\n";
    return true;
  }
  context.out << "<--DONE-->\n";
  parsed_.erase(filename);
  parsed_.try_emplace(filename, std::move(cell));
  return true;
}
bool rychkov::MainProcessor::reload(ParserContext& context)
{
  if (!eol(context.in))
  {
    return false;
  }
  std::map< std::string, ParseCell > new_parsed;
  for (const std::pair< const std::string, ParseCell >& file: parsed_)
  {
    CParseContext parse_context{context.out, context.err, file.first};
    std::ifstream in(file.first);
    if (!in)
    {
      context.err << "failed to reopen source file: \"" << file.first << "\"\n";
    }
    std::pair< decltype(new_parsed)::iterator, bool > cell_p = new_parsed.try_emplace(file.first,
          std::move(parse_context), last_stage_, include_dirs_);
    if (cell_p.second)
    {
      context.out << "<--PARSE: \"" << file.first << "\"-->\n";
      if (!cell_p.first->second.parse(in))
      {
        context.err << "failed to parse file \"" << file.first << "\" - stopping\n";
        return true;
      }
    }
  }
  context.out << "<--DONE-->\n";
  parsed_ = std::move(new_parsed);
  return true;
}
