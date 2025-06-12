#ifndef CODE_PARSERS_HPP
#define CODE_PARSERS_HPP

#include <iosfwd>
#include <stack>
#include <set>
#include <string>
#include <parser.hpp>
#include "content.hpp"

namespace rychkov
{
  struct CParseContext
  {
    std::ostream& err;
    std::string file;
    CParseContext* base = nullptr;
    size_t line = 0, symbol = 0;
    std::string last_line;
  };

  class Lexer;
  class CParser;
  class Preprocessor
  {
  public:
    Preprocessor();
    Preprocessor(Lexer& next);

    void parse(CParseContext& context, std::string line);

  private:
    struct CommandContext
    {
      std::istream& in;
      std::ostream& err;
      CParseContext& parse_context;

      void parse_error();
    };
    static Parser::map_type< CommandContext, Preprocessor > call_map;

    Lexer* next_;
    bool singleline_comment = false;
    bool multiline_comment = false;

    void flush(CParseContext& context, char c);

    bool include(CommandContext&);
    bool define(CommandContext&);
    bool pragma(CommandContext&);
    bool undef(CommandContext&);
  };

  class Lexer
  {
  public:
    Lexer();
    Lexer(CParser& next);

    void parse(CParseContext& context, std::string str);
    bool append(CParseContext& context, char c);
    bool flush(CParseContext& context);

  private:
    CParser* next_;
    bool literal_full_;
    std::variant< std::monostate, std::string, entities::Literal > buf_;

    bool append_new(CParseContext& context, char c);
    bool append_name(CParseContext& context, char c);
    bool append_literal(CParseContext& context, char c);
  };

  class CParser
  {
  public:
    static const std::vector< Operator > operators;

    CParser();

    bool append(CParseContext& context, char c);
    bool append(CParseContext& context, entities::Literal literal);
    bool append(CParseContext& context, std::string name);

    void print(std::ostream& out) const;
  private:
    std::vector< entities::Expression > program_ = {{}};
    std::set< std::pair< typing::Type, size_t > > base_types_; // data and declare-depth
    std::set< entities::Alias > aliases_;
    std::set< std::pair< entities::Variable, size_t > > variables_;
    std::set< entities::Variable > defined_functions_;
    std::set< std::pair< entities::Struct, size_t > > structs_;
    std::set< std::pair< entities::Union, size_t > > unions_;
    std::set< std::pair< entities::Enum, size_t > > enums_;
    std::stack< entities::Expression* > stack_;

    bool global_scope() const noexcept;
  };
  void log(CParseContext& context, std::string message);
}

#endif
