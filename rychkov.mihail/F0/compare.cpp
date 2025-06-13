#include "code_parsers.hpp"

bool rychkov::NameCompare::operator()(const struct_value& lhs, const struct_value& rhs) const
{
  return (lhs.first.name < rhs.first.name) || ((lhs.first.name == rhs.first.name) && (lhs.second > rhs.second));
}
bool rychkov::NameCompare::operator()(const base_type_value& lhs, const base_type_value& rhs) const
{
  return (lhs.first.name < rhs.first.name) || ((lhs.first.name == rhs.first.name) && (lhs.second > rhs.second));
}
bool rychkov::NameCompare::operator()(const base_type_value& lhs, const std::string& rhs) const
{
  return lhs.first.name < rhs;
}
bool rychkov::NameCompare::operator()(const std::string& lhs, const base_type_value& rhs) const
{
  return lhs < rhs.first.name;
}
