#include "cparser.hpp"

bool rychkov::CParser::parse_open_parenthesis(CParseContext& context)
{
  if (stack_.top()->full())
  {
    move_down();
    stack_.top()->operation = &*operators.find("()");
  }
  entities::Expression* temp = new entities::Expression;
  stack_.top()->operands.push_back(temp);
  stack_.push(temp);
  return true;
}
bool rychkov::CParser::parse_close_parenthesis(CParseContext& context)
{
  bool last_not_full = false;
  entities::Expression* last = stack_.top();
  while (entities::is_operator(*stack_.top()))
  {
    if (last_not_full)
    {
      log(context, "found not full operator during priority folding");
    }
    last_not_full = !stack_.top()->full();
    last = stack_.top();
    stack_.pop();
  }
  if (!stack_.top()->empty())
  {
    entities::Expression::operand bridge_child = std::move(stack_.top()->operands[0]);
    stack_.pop();
    stack_.top()->operands.back() = std::move(bridge_child);
  }
  else
  {
    stack_.pop();
  }
  if (!entities::is_operator(*stack_.top()) && last_not_full)
  {
    log(context, "found not full operator during priority folding");
  }
  // if is_decl || is_body
  if (stack_.top()->operation == &*operators.find("()"))
  {
    if (last->empty())
    {
      stack_.top()->operands.pop_back();
    }
    stack_.pop();
  }
  return true;
}
