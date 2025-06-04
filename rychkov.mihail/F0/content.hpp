#ifndef CONTENT_HPP
#define CONTENT_HPP

#include <string>
#include <vector>
#include <variant>
#include <memory>
#include <utility>

namespace rychkov
{
  namespace keywords
  {
    enum Keyword
    {
      Const,
      Volatile,
      Unsigned,
      Long,
      True,
      False,
      Enum,
      Struct,
      Union
    };
  }
  struct Operator
  {
    enum Type
    {
      unary,
      binary,
      ternary,
      multiple
    };

    Type type;
    std::string token;
    bool right_align = false;
    int priority = 0;
  };
  namespace typing
  {
    struct Type
    {
      enum LengthCategory
      {
        NO_LENGTH,
        SHORT,
        LONG,
        LONG_LONG
      };
      enum Category
      {
        Struct,
        Int,
        Pointer,
        Array,
        Function,
        Combination
      };

      std::string name;
      Category category = Combination;
      Type* base = nullptr;
      bool is_const = false;
      bool is_volatile = false;
      bool is_signed = false;
      bool is_unsigned = false;
      LengthCategory length_category = NO_LENGTH;
      bool array_has_length = false;
      size_t array_length = 0;
      std::vector< Type > function_parameters;
    };
  }
  template< class T >
  struct DinMemWrapper: std::unique_ptr< T >
  {
    using value_type = T;
    DinMemWrapper():
      std::unique_ptr< T >(new T)
    {}
    DinMemWrapper(const DinMemWrapper& rhs):
      std::unique_ptr< T >(new T{*rhs})
    {}
    DinMemWrapper(DinMemWrapper&&) = default;
    DinMemWrapper& operator=(const DinMemWrapper& rhs)
    {
      static_cast< std::unique_ptr< T >& >(*this) = new T{*rhs};
    }
    DinMemWrapper& operator=(DinMemWrapper&&) = default;
  };
  namespace entities
  {
    struct Expression;
    struct Body
    {
      std::vector< Expression > data;
    };
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
      bool inplace_defined = false;
      Body body;
    };
    struct Struct
    {
      std::string name;
      std::vector< Variable > fields;
      std::vector< Expression > defaults;
    };
    struct Enum
    {
      std::string name;
      std::vector< std::pair< std::string, int > > fields;
    };
    struct Union
    {
      std::string name;
      std::vector< Variable > fields;
      DinMemWrapper< Expression > value;
      size_t initialized_field = ~0ULL;
    };
    struct Alias
    {
      typing::Type type;
      std::string name;
    };
    struct Declaration
    {
      std::variant< Variable, Struct, Enum, Union, Alias, Function > data;
      DinMemWrapper< Expression > value;
    };
    struct Literal
    {
      enum Type
      {
        Char,
        String,
        Number,
        Logic
      };

      std::string literal;
      std::string suffix;
      Type type;
    };
    struct Expression
    {
      using operand = std::variant< DinMemWrapper< Expression >, Variable, Declaration, Literal >;
      Operator* operation;
      typing::Type result_type;
      std::vector< operand > operands;
    };
    struct Name
    {
      std::string name;
    };
  }
}

#endif

