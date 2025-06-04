#include "tokenizer.hpp"

#include "content_print.hpp"

std::vector< rychkov::Operator > rychkov::Tokenizer::operators = {
    {rychkov::Operator::unary, "+", true, 2},
    {rychkov::Operator::unary, "++", false, 1},
    {rychkov::Operator::unary, "++", true, 2},
    {rychkov::Operator::binary, "+", false, 4},
    {rychkov::Operator::unary, "sizeof", true, 2}
};

void rychkov::Tokenizer::print(std::ostream& err) const
{
  ContentPrinter printer{err};
  for (const entities::Expression& i: program_)
  {
    printer(i);
  }
}
