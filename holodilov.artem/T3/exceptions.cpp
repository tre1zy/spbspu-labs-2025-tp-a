#include "exceptions.hpp"

const char* holodilov::InvalidCommandException::what() const noexcept
{
  return "<INVALID COMMAND>\n";
}