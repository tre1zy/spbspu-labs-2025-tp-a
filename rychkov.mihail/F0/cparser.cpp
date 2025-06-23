#include "cparser.hpp"

#include <iostream>
#include <map>
#include "print_content.hpp"

using namespace std::literals::string_literals;

const std::set< rychkov::Operator, rychkov::NameCompare > rychkov::CParser::operators = {
      {rychkov::Operator::UNARY, rychkov::Operator::SPECIAL, "sizeof", false, false, false, 0}
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
bool rychkov::CParser::global_scope() const noexcept
{
  return (stack_.size() == 1) && (stack_.top()->empty());
}

void rychkov::CParser::append(CParseContext& context, entities::Literal literal)
{
  if (!type_parser_.empty())
  {
    if (literal.type != entities::Literal::Number)
    {
      log(context, "non-numeric literal cannot be a part of a type");
      return;
    }
    try
    {
      type_parser_.append(context, std::stoull(literal.literal, nullptr, 0));
    }
    catch (...)
    {
      log(context, "failed to parse numeric literal");
    }
    return;
  }
  if (global_scope() || stack_.top()->full())
  {
    log(context, "unexpected literal");
    return;
  }
  stack_.top()->operands.push_back(std::move(literal));
}
void rychkov::CParser::append(CParseContext& context, std::string name)
{
  decltype(base_types_)::const_iterator base_type_p = base_types_.find(name);
  if (base_type_p != base_types_.cend())
  {
    type_parser_.append(context, base_type_p->first);
    return;
  }
  decltype(aliases_)::const_iterator alias_p = aliases_.find(name);
  if (alias_p != aliases_.cend())
  {
    type_parser_.append(context, alias_p->type);
    return;
  }
  if (!type_parser_.empty())
  {
    type_parser_.append(context, name);
    return;
  }
  static const std::set< std::string > type_keywords = {"const", "volatile", "signed", "unsigned"};
  if (type_keywords.find(name) != type_keywords.end())
  {
    type_parser_.append(context, name);
    return;
  }

  decltype(variables_)::const_iterator var_p = variables_.find(name);
  if (var_p != variables_.cend())
  {
    if (stack_.top()->full())
    {
      log(context, "unexpected variable");
      return;
    }
    stack_.top()->operands.push_back(var_p->first);
    return;
  }

  static const std::map< std::string, void(CParser::*)(CParseContext&) > keywords = {
        {"typedef", &CParser::parse_typedef},
        {"struct", &CParser::parse_struct},
        {"return", &CParser::parse_return},
        {"if", &CParser::parse_if},
        {"while", &CParser::parse_while}
      };
  decltype(keywords)::const_iterator keyword_p = keywords.find(name);
  if (keyword_p != keywords.cend())
  {
    (this->*(keyword_p->second))(context);
    return;
  }

  if (stack_.top()->empty())
  {
    log(context, "unexpected name (" + name + ")");
    return;
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
        return;
      }
      log(context, "struct name duplicating (" + name + ")");
      return;
    }
  }
  log(context, "unexpected name (" + name + ")");
}

void rychkov::CParser::append(CParseContext& context, char c)
{
  if (!type_parser_.empty())
  {
    if (!type_parser_.ready() || (c == '*') || (c == '(') || (c == '['))
    {
      type_parser_.append(context, c);
    }
    else if (flush_type_parser(context))
    {
      append(context, c);
    }
    return;
  }
  using append_signature = void(CParser::*)(CParseContext&);
  using append_map = std::map< char, append_signature >;
  static const append_map dispatch_map = {
        {';', &CParser::parse_semicolon},
        {'{', &CParser::parse_open_brace},
        {'}', &CParser::parse_close_brace},
        {'(', &CParser::parse_open_parenthesis},
        {')', &CParser::parse_close_parenthesis},
        {'[', &CParser::parse_open_bracket},
        {']', &CParser::parse_close_bracket},
        {',', &CParser::parse_comma},
        {'?', &CParser::parse_question_mark},
        {':', &CParser::parse_colon}
      };

  append_map::const_iterator found = dispatch_map.find(c);
  if (found != dispatch_map.cend())
  {
    (this->*(found->second))(context);
    return;
  }
  log(context, "unknown symbol ('"s + c + "'; code = " + std::to_string(c) + ")");
}
bool rychkov::CParser::flush_type_parser(CParseContext& context)
{
  if (!type_parser_.empty())
  {
    if (!type_parser_.ready())
    {
      log(context, "type cannot be finished here");
      return false;
    }
    type_parser_.prepare();
    if (entities::is_decl(*stack_.top()))
    {
      entities::Declaration& decl = std::get< entities::Declaration >(stack_.top()->operands[0]);
      if (std::holds_alternative< entities::Alias >(decl.data))
      {
        entities::Alias& alias = std::get< entities::Alias >(decl.data);
        if (!alias.name.empty())
        {
          log(context, "duplicating types in typedef declaration");
        }
        else if (type_parser_.is_type())
        {
          log(context, "missing alias name");
        }
        else
        {
          entities::Variable temp = type_parser_.variable();
          alias = {std::move(temp.type), std::move(temp.name)};
        }
      }
      type_parser_.clear();
      return true;
    }
    if (type_parser_.is_function())
    {
      if (!global_scope())
      {
        log(context, "functions can be declared only in global scope");
        type_parser_.clear();
        return false;
      }
      *stack_.top() = entities::Declaration{type_parser_.function()};
      variables_.insert({type_parser_.variable(), stack_.size()});
    }
    else
    {
      if (!stack_.top()->empty())
      {
        log(context, "variable declaration cannot be placed here");
        type_parser_.clear();
        return false;
      }
      if (variables_.find({{{}, type_parser_.name()}, stack_.size()}) != variables_.end())
      {
        log(context, "variable name repeats in one scope");
        type_parser_.clear();
        return false;
      }
      *stack_.top() = entities::Declaration{type_parser_.variable()};
      variables_.insert({type_parser_.variable(), stack_.size()});
    }
  }
  type_parser_.clear();
  return true;
}
bool rychkov::CParser::append_empty(CParseContext& context)
{
  if (stack_.empty())
  {
    stack_.push(&program_.emplace_back());
    return true;
  }
  if (entities::is_body(*stack_.top()))
  {
    entities::Body& body = std::get< entities::Body >(stack_.top()->operands[0]);
    stack_.push(&body.data.emplace_back());
    return true;
  }
  if (entities::is_decl(*stack_.top()))
  {
    entities::Declaration& decl = std::get< entities::Declaration >(stack_.top()->operands[0]);
    if (decl.value != nullptr)
    {
      stack_.pop();
      return append_empty(context);
    }
    return true;
  }
  log(context, "cannot append any expr to this place");
  return false;
}
