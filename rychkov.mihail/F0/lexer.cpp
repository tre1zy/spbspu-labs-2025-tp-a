#include "code_parsers.hpp"

#include <iostream>
#include "content_print.hpp"

rychkov::Lexer::Lexer():
  next_(nullptr),
  literal_full_(false)
{}
rychkov::Lexer::Lexer(CParser& next):
  next_(&next),
  literal_full_(false)
{}

void rychkov::Lexer::parse(CParseContext& context, std::string str)
{
  for (char c: str)
  {
    append(context, c);
    context.symbol++;
    if (c == '\n')
    {
      context.line++;
      context.symbol = 0;
    }
  }
}
bool rychkov::Lexer::flush(CParseContext& context)
{
  if (std::holds_alternative< std::monostate >(buf_))
  {
    return false;
  }
  bool result = true;
  if (std::holds_alternative< std::string >(buf_))
  {
    if (next_ == nullptr)
    {
      std::cout << "<name> " << std::get< std::string >(buf_) << '\n';
    }
    else
    {
      result = next_->append(context, std::move(std::get< std::string >(buf_)));
    }
  }
  else if (std::holds_alternative< entities::Literal >(buf_))
  {
    if (next_ == nullptr)
    {
      std::cout << "<lit>  " << std::get< entities::Literal >(buf_) << '\n';
    }
    else
    {
      result = next_->append(context, std::get< entities::Literal >(buf_));
    }
  }
  literal_full_ = false;
  buf_.emplace< 0 >();
  return result;
}
bool rychkov::Lexer::append(CParseContext& context, char c)
{
  if (std::holds_alternative< entities::Literal >(buf_))
  {
    return append_literal(context, c);
  }
  if (std::holds_alternative< std::string >(buf_))
  {
    return append_name(context, c);
  }
  return append_new(context, c);
}
bool rychkov::Lexer::append_new(CParseContext& context, char c)
{
  if (std::isalpha(c))
  {
    buf_ = std::string{c};
  }
  else if (std::isdigit(c))
  {
    buf_ = entities::Literal{{c}, {}, entities::Literal::Number};
  }
  else if (c == '\'')
  {
    buf_ = entities::Literal{{c}, {}, entities::Literal::Char};
  }
  else if (c == '"')
  {
    buf_ = entities::Literal{{c}, {}, entities::Literal::String};
  }
  else if (!std::isspace(c))
  {
    if (next_ == nullptr)
    {
      std::cout << "<spec> " << c << '\n';
      return true;
    }
    return next_->append(context, c);
  }
  return true;
}
bool rychkov::Lexer::append_name(CParseContext& context, char c)
{
  std::string& str = std::get< std::string >(buf_);
  if (std::isalnum(c))
  {
    str += c;
    return true;
  }
  flush(context);
  return true;
}
bool rychkov::Lexer::append_literal(CParseContext& context, char c)
{
  entities::Literal& literal = std::get< entities::Literal >(buf_);
  if (std::isalnum(c))
  {
    if ((literal.type == entities::Literal::Number) && !literal_full_ && std::isalpha(c))
    {
      if ((literal.literal == "0x") || (literal.literal == "0b"))
      {
        log(context, "wrong numeric literal prefix");
        return false;
      }
      if ((literal.literal != "0") || ((c != 'x') && (c != 'b')))
      {
        literal_full_ = true;
      }
    }
    (literal_full_ ? literal.suffix : literal.literal) += c;
    return true;
  }
  if (literal_full_)
  {
    return flush(context);
  }
  switch (literal.type)
  {
  case entities::Literal::String:
    if (c == '"')
    {
      literal_full_ = true;
      return true;
    }
    literal.literal += c;
    return true;
  case entities::Literal::Char:
    if (c == '\'')
    {
      literal_full_ = true;
      return true;
    }
    literal.literal += c;
    return true;
  case entities::Literal::Number:
    if (c == '.')
    {
      literal.literal += c;
      return true;
    }
    return (c == '\'') || (flush(context) && append(context, c));
  }
  log(context, "unexpected symbol");
  return false;
}
