#include "type_parser.hpp"

#include <map>

void rychkov::TypeParser::append(CParseContext& context, std::string name)
{
  using append_signature = void(TypeParser::*)(CParseContext&);
  using append_map = std::map< std::string, append_signature >;
  static const append_map dispatch_map = {
        {"const", &TypeParser::append_const},
        {"volatile", &TypeParser::append_volatile},
        {"signed", &TypeParser::append_signed},
        {"unsigned", &TypeParser::append_unsigned}
      };

  append_map::const_iterator found = dispatch_map.find(name);
  if (found != dispatch_map.cend())
  {
    (this->*(found->second))(context);
    return;
  }

  if (stack_.top().data != &combined_)
  {
    if (can_be_named_param(stack_.top().data))
    {
      parameters_.back() = std::move(name);
      return;
    }
    log(context, "name \"" + name + "\" cannot be placed here");
    return;
  }
  if (!var_name_.empty())
  {
    log(context, "name repeated");
    return;
  }
  var_name_ = std::move(name);
  stack_.top().right_allign = true;
}
void rychkov::TypeParser::append_const(CParseContext& context)
{
  if (stack_.empty())
  {
    combined_ = {{}, typing::COMBINATION};
    combined_.is_const = true;
    stack_.push({&combined_});
  }
  else if (stack_.top().right_allign)
  {
    log(context, "const cannot be applied here");
  }
  else if (stack_.top().data->is_const)
  {
    log(context, "const repeated");
  }
  else
  {
    stack_.top().data->is_const = true;
  }
}
void rychkov::TypeParser::append_volatile(CParseContext& context)
{
  if (stack_.empty())
  {
    combined_ = {{}, typing::COMBINATION};
    combined_.is_volatile = true;
    stack_.push({&combined_});
  }
  if (stack_.top().right_allign)
  {
    log(context, "volatile cannot be applied here");
  }
  if (stack_.top().data->is_volatile)
  {
    log(context, "volatile repeated");
  }
  else
  {
    stack_.top().data->is_volatile = true;
  }
}
void rychkov::TypeParser::append_signed(CParseContext&)
{}
void rychkov::TypeParser::append_unsigned(CParseContext&)
{}
