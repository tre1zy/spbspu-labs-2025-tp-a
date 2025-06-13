#include "code_parsers.hpp"

#include <iostream>
#include "content_print.hpp"

using namespace std::literals::string_literals;

const std::vector< rychkov::Operator > rychkov::CParser::operators = {
      {rychkov::Operator::unary, rychkov::Operator::arithmetic, "+", false, true, 2},
      {rychkov::Operator::unary, rychkov::Operator::arithmetic, "++", true, false, 1},
      {rychkov::Operator::unary, rychkov::Operator::arithmetic, "++", true, true, 2},
      {rychkov::Operator::binary, rychkov::Operator::arithmetic, "+", false, false, 4},
      {rychkov::Operator::unary, rychkov::Operator::special, "sizeof", false, true, 2}
    };

void rychkov::log(CParseContext& context, std::string message)
{
  for (const CParseContext* file = &context; file != nullptr; file = file->base)
  {
    context.err << "In file \"" << file->file << "\" (" << context.line + 1 << ':' << context.symbol + 1 << ")\n";
  }
  context.err << "\tError: " << message << "\n\n  " << context.last_line << '\n';
  context.err << "  " << std::string(context.symbol, '-') << "^\n";
}

rychkov::CParser::CParser():
  program_{{}}
{
  stack_.push(&program_[0]);
}

void rychkov::CParser::print(std::ostream& out) const
{
  ContentPrinter printer{out};
  for (const entities::Expression& i: program_)
  {
    printer(i);
  }
}
bool rychkov::CParser::global_scope() const noexcept
{
  return (stack_.size() == 1) && (stack_.top()->empty());
}

bool rychkov::CParser::append(CParseContext& context, entities::Literal literal)
{
  if (global_scope() || stack_.top()->full())
  {
    log(context, "unexpected literal");
    return false;
  }
  stack_.top()->operands.push_back(std::move(literal));
  return true;
}
bool rychkov::CParser::append(CParseContext& context, std::string name)
{
  if (name == "const")
  {
    // type
  }
  else if (name == "struct")
  {
    if (global_scope() || stack_.top()->empty())
    {
      stack_.top()->operands.push_back(entities::Declaration{entities::Struct{}});
      return true;
    }
    log(context, "struct can't be declared in this context");
  }
  // if ()
  // name
  if (stack_.top()->empty())
  {
    log(context, "unexpected name (" + name + ")");
    return false;
  }
  if (entities::is_decl(*stack_.top()))
  {
    entities::Declaration& decl = std::get< entities::Declaration >(stack_.top()->operands[0]);
    if (std::holds_alternative< entities::Struct >(decl.data))
    {
      entities::Struct& data = std::get< entities::Struct >(decl.data);
      if (data.name.empty())
      {
        data.name = std::move(name);
        return true;
      }
      log(context, "struct name duplicating (" + name + ")");
      return false;
    }
  }
  return true;
}
bool rychkov::CParser::append(CParseContext& context, char c)
{
  using append_signature = bool(CParser::*)(CParseContext&);
  using append_map = std::map< char, append_signature >;
  static const append_map dispatch_map = {
        {';', &CParser::parse_semicolon},
        {'{', &CParser::parse_open_brace},
        {'}', &CParser::parse_close_brace}
      };

  append_map::const_iterator found = dispatch_map.find(c);
  if (found != dispatch_map.cend())
  {
    return (this->*(found->second))(context);
  }
  log(context, "unknown symbol ('"s + c + "'; code = " + std::to_string(c) + ")");
  return false;
}
bool rychkov::CParser::parse_semicolon(CParseContext& context)
{
  for (; stack_.top()->operation != nullptr; stack_.pop()) // pop operators
  {}
  if (stack_.top()->empty())
  {
    stack_.pop();
  }
  if (stack_.empty())
  {
    return append_empty(context);
  }
  if (entities::is_body(*stack_.top())) //append new empty to body
  {
    entities::Body& body = std::get< entities::Body >(stack_.top()->operands[0]);
    stack_.push(&body.data.emplace_back());
    return true;
  }
  if (entities::is_decl(*stack_.top()))
  {
    entities::Declaration& decl = std::get< entities::Declaration >(stack_.top()->operands[0]);
    if (std::holds_alternative< entities::Struct >(decl.data))
    {
      entities::Struct& data = std::get< entities::Struct >(decl.data);
      if (data.name.empty())
      {
        log(context, "struct must have name");
        return false;
      }
      // add struct to dict
      stack_.pop();
      structs_.insert(std::make_pair(data, stack_.size()));
      base_types_.insert(std::make_pair(typing::Type{data.name, typing::Type::Struct}, stack_.size()));
      return append_empty(context);
    }
  }
  stack_.push(&program_.emplace_back()); //append new empty to program
  return true;
}
bool rychkov::CParser::append_empty(CParseContext& context)
{
  if (stack_.empty())
  {
    stack_.push(&program_.emplace_back()); //append new empty to program
    return true;
  }
  if (entities::is_body(*stack_.top())) //append new empty to body
  {
    entities::Body& body = std::get< entities::Body >(stack_.top()->operands[0]);
    stack_.push(&body.data.emplace_back());
    return true;
  }
  log(context, "cannot append any expr to this place");
  return false;
}
