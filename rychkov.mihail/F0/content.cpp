#include "content.hpp"

bool rychkov::entities::Expression::empty() const noexcept
{
  return (operation == nullptr) && operands.empty();
}
bool rychkov::entities::Expression::full() const noexcept
{
  if (operation == nullptr)
  {
    return !operands.empty();
  }
  if (operation->type == -1)
  {
    return false;
  }
  return operands.size() >= operation->type;
}
bool rychkov::entities::is_body(const entities::Expression& expr)
{
  return (expr.operation == nullptr) && !expr.operands.empty()
      && std::holds_alternative< entities::Body >(expr.operands[0]);
}
bool rychkov::entities::is_decl(const entities::Expression& expr)
{
  return (expr.operation == nullptr) && !expr.operands.empty()
      && std::holds_alternative< entities::Declaration >(expr.operands[0]);
}
