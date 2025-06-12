#include "code_parsers.hpp"

#include <iostream>
#include <sstream>

rychkov::Parser::map_type< rychkov::Preprocessor::CommandContext, rychkov::Preprocessor >
    rychkov::Preprocessor::call_map = {
      {"include", &rychkov::Preprocessor::include},
      {"define", &rychkov::Preprocessor::define},
      {"pragma", &rychkov::Preprocessor::pragma},
      {"undef", &rychkov::Preprocessor::undef}
    };

rychkov::Preprocessor::Preprocessor():
  next_{nullptr}
{}
rychkov::Preprocessor::Preprocessor(Lexer& next):
  next_{&next}
{}

void rychkov::Preprocessor::flush(CParseContext& context, char c)
{
  if (singleline_comment || multiline_comment)
  {
    return;
  }
  if (next_ == nullptr)
  {
    std::cout << c;
  }
  else
  {
    next_->append(context, c);
  }
}
void rychkov::Preprocessor::parse(CParseContext& context, std::string line)
{
  context.last_line = std::move(line);
  std::istringstream in(context.last_line);
  if (!singleline_comment && !multiline_comment && ((in >> std::noskipws >> std::ws).peek() == '#'))
  {
    in.get();
    context.symbol++;
    CommandContext cmd_context{in, context.err, context};
    rychkov::Parser::parse(cmd_context, *this, rychkov::Preprocessor::call_map);
    if (!in)
    {
      log(context, "unknown preprocessor command");
    }
  }
  else
  {
    char c = '\0', prev = '\0';
    while (in.get(c))
    {
      if (!multiline_comment && prev == '/')
      {
        if (c == '/')
        {
          singleline_comment = true;
          break;
        }
        else if (c == '*')
        {
          multiline_comment = true;
          prev = '\0';
        }
        else
        {
          flush(context, '/');
        }
      }
      if (multiline_comment && (prev == '*'))
      {
        if (c == '/')
        {
          multiline_comment = false;
          prev = '\0';
          context.symbol++;
          continue;
        }
        else
        {
          flush(context, '*');
        }
      }
      if ((c != '/') || (multiline_comment && (c == '*')))
      {
        flush(context, c);
      }
      prev = c;
      context.symbol++;
    }
  }
  if (!multiline_comment && (context.last_line.empty() || (context.last_line.back() != '\\')))
  {
    if (next_ == nullptr)
    {
      std::cout << '\n';
    }
    else
    {
      next_->append(context, '\n');
      next_->flush(context);
    }
    singleline_comment = false;
  }
  context.line++;
  context.symbol = 0;
}
void rychkov::Preprocessor::CommandContext::parse_error()
{
  in.setstate(std::ios::failbit);
}
bool rychkov::Preprocessor::include(CommandContext& context)
{
  log(context.parse_context, "#include is unready");
  return true;
}
bool rychkov::Preprocessor::define(CommandContext& context)
{
  log(context.parse_context, "#define is unready");
  return true;
}
bool rychkov::Preprocessor::pragma(CommandContext& context)
{
  log(context.parse_context, "#pragma is unsupported");
  return true;
}
bool rychkov::Preprocessor::undef(CommandContext& context)
{
  log(context.parse_context, "#undef is unready");
  return true;
}
