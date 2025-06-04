#include "content_print.hpp"

#include <iostream>

void rychkov::ContentPrinter::print_indent()
{
  for (size_t i = 0; i < indent_; i++)
  {
    out << '\t';
  }
}
void rychkov::ContentPrinter::print_empty()
{
  print_indent();
  out << "<EMPTY>\n";
}
void rychkov::ContentPrinter::operator()(const entities::Variable& var)
{
  print_indent();
  out << "[variable] ";
  print_left(var.type);
  out << ' ' << var.name;
  print_right(var.type);
  out << '\n';
}
void rychkov::ContentPrinter::operator()(const entities::Function& func)
{
  print_indent();
  out << "[function] ";
  print_left(func.type);
  out << ' ' << func.name;
  out << '(';
  char comma[3] = "\0 ";
  for (size_t i = 0; i < func.type.function_parameters.size(); i++)
  {
    out << comma;
    print_left(func.type.function_parameters[i]);
    out << ' ' << func.parameters[i];
    print_right(func.type.function_parameters[i]);
    comma[0] = ',';
  }
  out << ')';
  print_right_parenthesis(func.type);
  print_right(*func.type.base);
  out << '\n';
  if (func.inplace_defined)
  {
    operator()(func.body);
  }
}
void rychkov::ContentPrinter::operator()(const entities::Body& body)
{
  if (body.data.empty())
  {
    print_indent();
    out << "{}\n";
  }
  else
  {
    print_indent();
    out << "{\n";
    indent_++;
    for (const entities::Expression& i: body.data)
    {
      operator()(i);
    }
    indent_--;
    print_indent();
    out << "}\n";
  }
}
void rychkov::ContentPrinter::operator()(const entities::Struct& structure)
{
  print_indent();
  out << "[struct] " << structure.name << '\n';
  indent_++;
  for (size_t i = 0; i < structure.fields.size(); i++)
  {
    operator()(structure.fields[i]);
    if (!structure.defaults[i].operands.empty())
    {
      indent_++;
      operator()(structure.defaults[i]);
      indent_--;
    }
  }
  indent_--;
}
void rychkov::ContentPrinter::operator()(const entities::Enum& structure)
{
  print_indent();
  out << "[enum] " << structure.name << '\n';
  indent_++;
  for (const std::pair< std::string, int >& i: structure.fields)
  {
    print_indent();
    out << i.first << " = " << i.second << '\n';
  }
  indent_--;
}
void rychkov::ContentPrinter::operator()(const entities::Union& structure)
{
  print_indent();
  out << "[union] " << structure.name << '\n';
  indent_++;
  for (size_t i = 0; i < structure.fields.size(); i++)
  {
    operator()(structure.fields[i]);
    if (i == structure.initialized_field)
    {
      indent_++;
      operator()(*structure.value);
      indent_--;
    }
  }
  indent_--;
}
void rychkov::ContentPrinter::operator()(const entities::Alias& alias)
{
  print_indent();
  out << "[typedef] ";
  print_left(alias.type);
  out << ' ' << alias.name;
  print_right(alias.type);
  out << '\n';
}
void rychkov::ContentPrinter::operator()(const entities::Declaration& decl)
{
  print_indent();
  out << "[declaration]\n";
  indent_++;
  std::visit(*this, decl.data);
  indent_--;
  if (std::holds_alternative< entities::Variable >(decl.data) && !decl.value->operands.empty())
  {
    print_indent();
    out << "with default:\n";
    indent_++;
    operator()(*decl.value);
    indent_--;
  }
}
void rychkov::ContentPrinter::operator()(const entities::Literal& literal)
{
  print_indent();
  switch (literal.type)
  {
  case entities::Literal::String:
    out << "[string] \"" << literal.literal << '"' << literal.suffix << '\n';
    break;
  case entities::Literal::Char:
    out << "[char] '" << literal.literal << '\'' << literal.suffix << '\n';
    break;
  case entities::Literal::Number:
    out << "[number] " << literal.literal << literal.suffix << '\n';
    break;
  case entities::Literal::Logic:
    out << "[logic] " << literal.literal << '\n';
    break;
  }
}
