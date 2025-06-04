#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <stack>
#include <set>
#include <iosfwd>
#include "content.hpp"

namespace rychkov
{
  class Tokenizer
  {
  public:
    void append(char c);
    void print(std::ostream& err) const;
  private:
    using value_type = std::variant< entities::Expression, entities::Name, keywords::Keyword >;
    using pointer = std::unique_ptr< value_type >;
    value_type* tail_ = nullptr;
    std::vector< entities::Expression > program_;
    std::stack< value_type > waitlist_;
    std::set< typing::Type > types_;
    std::set< entities::Variable > active_variables_;
    static std::vector< Operator > operators;
  };
}

#endif
