#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <iosfwd>
#include <stack>
#include <set>
#include <string>
#include "content.hpp"

namespace rychkov
{
  class Tokenizer
  {
  public:
    static std::vector< Operator > operators;

    Tokenizer():
      program_{{nullptr, {}, {}}}
    {
      stack_.push(&program_[0]);
    }

    bool append(std::ostream& err, char c);
    bool append(std::ostream& err, entities::Literal literal);
    bool append(std::ostream& err, std::string name);

    void merge(std::ostream& err, typing::Type& parent, const typing::Type& child);
    void print(std::ostream& out) const;
    void log(std::ostream& err, std::string message) const;
  private:
    std::vector< entities::Expression > program_ = {{nullptr, {}, {}}};
    std::set< std::pair< typing::Type, size_t > > base_types_; // data and declare-depth
    std::set< entities::Alias > aliases_;
    std::set< entities::Cast > casts_;
    std::set< std::pair< entities::Variable, size_t > > variables_;
    std::set< std::pair< entities::Struct, size_t > > structs_;
    std::set< std::pair< entities::Union, size_t > > unions_;
    std::set< std::pair< entities::Enum, size_t > > enums_;
    std::stack< entities::Expression* > stack_;

    bool global_scope() const noexcept;
  };
}

#endif
