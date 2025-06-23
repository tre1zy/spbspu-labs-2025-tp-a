#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <set>
#include <map>
#include <vector>
#include <variant>

#include "content.hpp"
#include "compare.hpp"
#include "log.hpp"

namespace rychkov
{
  class CParser;
  class Lexer
  {
  public:
    enum TypeKeyword
    {
      CONST,
      VOLATILE,
      SIGNED,
      UNSIGNED
    };
    Lexer(CParser* next = nullptr);

    void append_name(CParseContext& context, std::string name);
    void append_number(CParseContext& context, std::string name);
    void append_string_literal(CParseContext& context, std::string name);
    void append_char_literal(CParseContext& context, std::string name);
    void append(CParseContext& context, char c);
    void flush(CParseContext& context);

  private:
    using operator_value = const std::vector< Operator >*;

    const std::set< std::vector< Operator >, NameCompare > cases_;
    const std::map< std::string, TypeKeyword > type_keywords_;
    const std::map< std::string, void(CParser::*)(CParseContext&) > keywords_;

    CParser* next_;
    std::variant< std::monostate, operator_value, entities::Literal > buf_;

    void append_new(CParseContext& context, char c);
    void append_operator(CParseContext& context, char c);
  };
}

#endif
