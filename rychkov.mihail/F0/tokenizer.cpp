#include "tokenizer.hpp"

#include <iostream>
#include <sstream>
#include "content_print.hpp"

std::vector< rychkov::Operator > rychkov::Tokenizer::operators = {
    {rychkov::Operator::unary, rychkov::Operator::arithmetic, "+", false, true, 2},
    {rychkov::Operator::unary, rychkov::Operator::arithmetic, "++", true, false, 1},
    {rychkov::Operator::unary, rychkov::Operator::arithmetic, "++", true, true, 2},
    {rychkov::Operator::binary, rychkov::Operator::arithmetic, "+", false, false, 4},
    {rychkov::Operator::unary, rychkov::Operator::special, "sizeof", false, true, 2}
};

void rychkov::Tokenizer::print(std::ostream& out) const
{
  ContentPrinter printer{out};
  for (const entities::Expression& i: program_)
  {
    printer(i);
  }
}
void rychkov::Tokenizer::log(std::ostream& err, std::string message) const
{
  err << "Error: " << message << '\n';
}
void rychkov::Tokenizer::merge(std::ostream& err, typing::Type& parent, const typing::Type& child)
{
  switch (parent.category)
  {
  case typing::Type::Pointer:
    break;
  case typing::Type::Array:
    break;
  case typing::Type::Function:
  case typing::Type::Int:
  case typing::Type::Struct:
    break;
  case typing::Type::Combination:
    break;
  }
  /*if (!parent.name.empty() && !child.name.empty())
  {
    log(err, "type cannot consist of two: " + lhs.name + " and " + rhs.name);
    return;
  }*/
  /*
  if ((lhs.base != nullptr) && (rhs.base != nullptr))
  {
    std::ostringstream lhs_type, rhs_type;
    ContentPrinter{lhs_type}(lhs);
    ContentPrinter{rhs_type}(rhs);
    log(err, "type cannot consist of two: " + lhs_type.str() + " and " + rhs_type.str());
    return;
  }
  lhs.is_const = lhs.is_const || rhs.is_const;
  lhs.is_volatile = lhs.is_volatile || rhs.is_volatile;
  lhs.is_signed = lhs.is_signed || rhs.is_signed;
  lhs.is_unsigned = lhs.is_unsigned || rhs.is_unsigned;
  if (lhs.is_signed && lhs.is_unsigned)
  {
    log(err, "type connot be signed and unsigned at the same type");
    return;
  }
  if (rhs.length_category != typing::Type::NO_LENGTH)
  {
    lhs.length_category = rhs.length_category;
  }
  lhs.array_has_length = lhs.array_has_length || rhs.array_has_length;*/
}
bool rychkov::Tokenizer::global_scope() const noexcept
{
  return (stack_.size() == 1) && (stack_.top()->empty());
}
bool rychkov::Tokenizer::append(std::ostream& err, entities::Literal literal)
{
  if (global_scope() || stack_.top()->full())
  {
    log(err, "unexpected literal");
    return false;
  }
  stack_.top()->operands.push_back(std::move(literal));
  return true;
}
bool rychkov::Tokenizer::append(std::ostream& err, std::string name)
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
    log(err, "struct can't be declared in this context");
  }
  // name
  if (stack_.top()->empty())
  {
    log(err, "unexpected name");
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
      log(err, "struct name duplicating");
      return false;
    }
  }
  return true;
}
bool rychkov::Tokenizer::append(std::ostream& err, char c)
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
      log(err, "braced-enclosed body cannot be alone in global scope");
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
          log(err, "struct must have name");
          return false;
        }
        decl.value = {nullptr, {}, {entities::Body{}}};
        stack_.push(&*decl.value);
        return true;
      }
      return true;
    }
    log(err, "unexpected braced-enclosed body");
    return false;
  }
  else if (c == '}')
  {
    //
  }
  log(err, "unknown symbol");
  return false;
}
