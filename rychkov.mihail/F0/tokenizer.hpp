#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string>
#include <stack>
#include <set>
#include <vector>
#include <variant>
#include <parser.hpp>

namespace rychkov
{
  namespace keywords
  {
    enum Keyword
    {
      Const,
      Volatile,
      True,
      False,
      Enum,
      Struct,
      Union
    };
  }
  namespace operators
  {
    enum OperatorType
    {
      unary,
      binary,
      ternary,
      multiple
    };
    struct Operator
    {
      OperatorType type;
      std::string token;
      bool right_allign = false;
      int priority = 0;
    };
  }
  namespace typing
  {
    enum TypeLengthCategory
    {
      none,
      byte,
      word,
      dword,
      qword
    };
    struct Type
    {
      std::string name;
      bool is_const = false;
      bool is_volatile = false;
      bool is_unsigned = false;
      TypeLengthCategory length = none;
      Type* pointer_base = nullptr;
      bool is_array = false;
      size_t array_length = 0;
      bool is_function = false;
      std::vector< Type > function_parameters;
    };
    struct Alias
    {
      Type type;
      std::string name;
    };
  }
  namespace entities
  {
    struct Variable
    {
      typing::Type type;
      std::string name;
    };
    struct Function
    {
      typing::Type type;
      std::string name;
      std::vector< std::string > parameters;
    };
    enum StructType
    {
      structure,
      enumeration,
      union_like
    };
    struct Struct
    {
      StructType type;
      std::string name;
      std::vector< Variable > fields;
    };
    struct Declaration
    {
      std::variant< Variable, Type, Function > data;
    };
    struct Literal
    {
      std::string literal;
      std::string suffix;
      bool is_string = false;
      bool is_char = false;
      bool is_number = false;
      bool is_logic = false;
    };
    struct Expression
    {
      Operator* operation;
      Type result_type;
      std::vector< std::variant< Expression, Variable, Declaration, Literal > > operands;
    };
    struct Name
    {
      std::string name;
    };
  }
  class Tokenizer
  {
  public:
    ParserContext& context;

    void append(char c);
    void print();
  private:
    using value_type = std::variant< entities::Expression, entities::Name, keywords::Keyword >;
    value_type* head = nullptr;
    std::stack< value_type > waitlist;
    std::set< Type > types_;
    std::set< Variable > active_variables_;
    static std::vector< Operator > operators = {
        {unary, "+", true, 2},
        {unary, "++", false, 1},
        {unary, "++", true, 2},
        {binary, "+", false, 4},
        {unary, "sizeof", true, 2}
    };
  };
}

#endif
