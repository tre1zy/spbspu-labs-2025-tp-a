#ifndef CONTENT_PRINT_HPP
#define CONTENT_PRINT_HPP

#include <iosfwd>
#include "content.hpp"

namespace rychkov
{
  class ContentPrinter
  {
  public:
    std::ostream& out;
    ContentPrinter(std::ostream& ostream):
      out(ostream),
      indent_(0)
    {}

    void operator()(const typing::Type& type);
    void print_left(const typing::Type& type);
    void print_right(const typing::Type& type);

    void operator()(const entities::Variable& var);
    void operator()(const entities::Function& func);
    void operator()(const entities::Body& body);
    void operator()(const entities::Struct& structure);
    void operator()(const entities::Enum& structure);
    void operator()(const entities::Union& structure);
    void operator()(const entities::Alias& alias);
    void operator()(const entities::Literal& literal);
    void operator()(const entities::Declaration& decl);

    void operator()(const DinMemWrapper< entities::Expression >& root);
    void operator()(const entities::Expression& root);
    void operator()(const entities::Expression::operand& operand);

    void print_indent();
    void print_empty();
  private:
    size_t indent_;
    void print_left_parenthesis(const typing::Type& parent);
    void print_right_parenthesis(const typing::Type& parent);
  };
}

#endif
