#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <stdexcept>

namespace holodilov
{
  class InvalidCommandException final: public std::exception
  {
  public:
    const char* what() const noexcept override;
  };
}

#endif
