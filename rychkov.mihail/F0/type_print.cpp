#include "content_print.hpp"

#include <iostream>

void rychkov::ContentPrinter::print_left_parenthesis(const typing::Type& parent)
{
  if (parent.base == nullptr)
  {
    return;
  }
  if ((parent.category == typing::Type::Array) && (parent.base->category == typing::Type::Array))
  {
    return;
  }
  if ((parent.base->category == typing::Type::Function) || (parent.base->category == typing::Type::Array))
  {
    out << '(';
  }
}
void rychkov::ContentPrinter::print_right_parenthesis(const typing::Type& parent)
{
  if (parent.base == nullptr)
  {
    return;
  }
  if ((parent.category == typing::Type::Array) && (parent.base->category == typing::Type::Array))
  {
    return;
  }
  if ((parent.base->category == typing::Type::Function) || (parent.base->category == typing::Type::Array))
  {
    out << ')';
  }
}
void rychkov::ContentPrinter::print_left(const typing::Type& type)
{
  if (type.base != nullptr)
  {
    print_left(*type.base);
  }
  switch (type.category)
  {
  case typing::Type::Struct:
    out << type.name;
    break;
  case typing::Type::Int:
    if (type.is_signed)
    {
      out << "signed ";
    }
    if (type.is_unsigned)
    {
      out << "unsigned ";
    }
    switch (type.length_category)
    {
    case typing::Type::SHORT:
      out << "short ";
      break;
    case typing::Type::LONG:
      out << "long ";
      break;
    case typing::Type::LONG_LONG:
      out << "long long ";
      break;
    }
    out << type.name;
    break;
  case typing::Type::Pointer:
    print_left_parenthesis(type);
    out << '*';
    if (type.is_const)
    {
      out << " const";
    }
    if (type.is_volatile)
    {
      out << " volatile";
    }
    break;
  case typing::Type::Array:
    print_left_parenthesis(type);
    break;
  case typing::Type::Combination:
    if (type.is_const)
    {
      out << " const";
    }
    if (type.is_volatile)
    {
      out << " volatile";
    }
    break;
  }
}
void rychkov::ContentPrinter::print_right(const typing::Type& type)
{
  if (type.category == typing::Type::Function)
  {
    out << '(';
    char comma[3] = "\0\0";
    for (const typing::Type& i: type.function_parameters)
    {
      out << comma;
      operator()(i);
      comma[0] = ',';
      comma[1] = ' ';
    }
    out << ')';
    print_right_parenthesis(type);
  }
  else if (type.category == typing::Type::Pointer)
  {
    print_right_parenthesis(type);
  }
  else if (type.category == typing::Type::Array)
  {
    out << '[';
    if (type.array_has_length)
    {
      out << type.array_length;
    }
    out << ']';
    print_right_parenthesis(type);
  }
  if (type.base != nullptr)
  {
    print_right(*type.base);
  }
}
void rychkov::ContentPrinter::operator()(const typing::Type& type)
{
  print_left(type);
  print_right(type);
}
