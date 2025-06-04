#include "content_print.hpp"

#include <iostream>

void rychkov::ContentPrinter::operator()(const entities::Expression& expr)
{
  print_empty();
}
