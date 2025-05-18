#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <string>
#include <utility>
#include <map>
#include <limits>

namespace rychkov
{
  struct ParserContext
  {
    std::istream& in;
    std::ostream& out;
    std::ostream& err;

    bool eol();
    void parse_error();
  };
  template< class Proc >
  class Parser
  {
  public:
    using parser_processor = Proc;
    using call_signature = bool(parser_processor::*)(ParserContext&);
    using map_type = std::map< std::string, call_signature >;

    parser_processor processor;
    ParserContext context;

    template< class P = parser_processor, class M = map_type >
    Parser(ParserContext parse_context, P proc, M&& call_map):
      processor(std::forward< P >(proc)),
      context(parse_context),
      call_map_(std::forward< M >(call_map)),
      has_own_parser_(false)
    {}
    template< class P = parser_processor, class M = map_type >
    Parser(ParserContext parse_context, P proc, M&& call_map, call_signature own_parser):
      processor(std::forward< P >(proc)),
      context(parse_context),
      call_map_(std::forward< M >(call_map)),
      has_own_parser_(true),
      extern_parser_(own_parser)
    {}

    bool available() const noexcept
    {
      return !call_map_.empty() && context.in;
    }
    bool run()
    {
      if (has_own_parser_)
      {
        if ((processor.*extern_parser_)(context))
        {
          return available();
        }
      }
      std::string command;
      if (context.in >> command)
      {
        typename map_type::const_iterator call_p = call_map_.find(command);
        try
        {
          if ((call_p != call_map_.end()) && ((processor.*(call_p->second))(context)))
          {
            return available();
          }
        }
        catch(...)
        {}
        context.parse_error();
      }
      return available();
    }
  private:
    const map_type call_map_;
    bool has_own_parser_;
    call_signature extern_parser_;
  };
}

#endif
