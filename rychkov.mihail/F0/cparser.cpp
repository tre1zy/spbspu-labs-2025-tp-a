#include "code_parsers.hpp"

#include <iostream>
#include "content_print.hpp"

const std::vector< rychkov::Operator > rychkov::CParser::operators = {
      {rychkov::Operator::unary, rychkov::Operator::arithmetic, "+", false, true, 2},
      {rychkov::Operator::unary, rychkov::Operator::arithmetic, "++", true, false, 1},
      {rychkov::Operator::unary, rychkov::Operator::arithmetic, "++", true, true, 2},
      {rychkov::Operator::binary, rychkov::Operator::arithmetic, "+", false, false, 4},
      {rychkov::Operator::unary, rychkov::Operator::special, "sizeof", false, true, 2}
    };

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
void rychkov::log(CParseContext& context, std::string message)
{
  for (const CParseContext* file = &context; file != nullptr; file = file->base)
  {
    context.err << "In file \"" << file->file << "\" (" << context.line + 1 << ':' << context.symbol + 1 << ")\n";
  }
  context.err << "\tError: " << message << '\n';
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
  // name
  if (stack_.top()->empty())
  {
    log(context, "unexpected name");
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
      log(context, "struct name duplicating");
      return false;
    }
  }
  return true;
}
bool rychkov::CParser::append(CParseContext& context, char c)
{
  if (c == ';')
  {
    // finish him...
    for (; stack_.top()->operation != nullptr; stack_.pop())
    {}
    if (entities::is_body(*stack_.top()))
    {
      entities::Body& body = std::get< entities::Body >(stack_.top()->operands[0]);
      stack_.top() = &body.data.emplace_back();
      return true;
    }
    stack_.top() = &program_.emplace_back();
    return true;
  }
  else if (c == '{')
  {
    if (global_scope())
    {
      log(context, "braced-enclosed body cannot be alone in global scope");
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
          log(context, "struct must have name");
          return false;
        }
        decl.value = entities::Body{};
        stack_.push(&*decl.value);
        return true;
      }
      return true;
    }
    log(context, "unexpected braced-enclosed body");
    return false;
  }
  else if (c == '}')
  {
    //
  }
  log(context, "unknown symbol");
  return false;
}
