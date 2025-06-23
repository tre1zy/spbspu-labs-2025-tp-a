#include "cparser.hpp"

void rychkov::CParser::fold(CParseContext& context, const Operator* reference)
{
  if ((reference != nullptr) && entities::is_bridge(*stack_.top()))
  {
    move_up_down();
    stack_.top()->operation = reference;
    return;
  }
  entities::Expression* last = stack_.top();
  while (entities::is_operator(*stack_.top()))
  {
    if ((stack_.top()->operation == &inline_if) && (reference == &inline_if))
    {
      return;
    }
    if (!stack_.top()->full())
    {
      log(context, "found not full operator during priority folding");
    }
    if ((reference != nullptr) && ((stack_.top()->operation->priority > reference->priority)
        || ((stack_.top()->operation->priority == reference->priority) && reference->right_align)))
    {
      move_down();
      stack_.top()->operation = reference;
      return;
    }
    last = stack_.top();
    calculate_type(context, *last);
    stack_.pop();
  }
  if (reference != nullptr)
  {
    stack_.push(last);
    move_up();
    stack_.top()->operation = reference;
    return;
  }
}
rychkov::entities::Expression* rychkov::CParser::move_up()
{
  entities::Expression* temp = new entities::Expression{std::move(*stack_.top())};
  stack_.top()->operation = nullptr;
  stack_.top()->operands.push_back(temp);
  return temp;
}
void rychkov::CParser::move_down()
{
  entities::Expression* temp = new entities::Expression{nullptr, {std::move(stack_.top()->operands.back())}};
  stack_.top()->operands.back() = temp;
  stack_.push(temp);
}
void rychkov::CParser::move_up_down()
{
  stack_.push(move_up());
}

void rychkov::CParser::clear_scope()
{
  clear_scope(variables_);
  clear_scope(structs_);
  clear_scope(unions_);
  clear_scope(enums_);
}
template< class T >
void rychkov::CParser::clear_scope(T& pair_set)
{
  typename T::iterator pos = pair_set.begin();
  while (pos != pair_set.end())
  {
    if (pos->second <= stack_.size())
    {
      pos = pair_set.upper_bound(pos->first.name);
      continue;
    }
    typename T::iterator to = pair_set.lower_bound({pos->first, stack_.size()});
    pos = pair_set.erase(pos, to);
  }
}
