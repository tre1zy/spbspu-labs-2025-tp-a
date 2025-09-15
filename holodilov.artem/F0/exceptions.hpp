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

  class DictionaryNotFoundException final: public std::exception
  {
  public:
    const char* what() const noexcept override;
  };

  class DictionaryExportException final: public std::exception
  {
  public:
    const char* what() const noexcept override;
  };

  class DictionaryAlreadyExistsException final: public std::exception
  {
  public:
    const char* what() const noexcept override;
  };

  class WordNotFoundException final: public std::exception
  {
  public:
    const char* what() const noexcept override;
  };

  class DictionaryFileNotFoundException final: public std::exception
  {
  public:
    const char* what() const noexcept override;
  };

  class InvalidDictionaryFileException final: public std::exception
  {
  public:
    const char* what() const noexcept override;
  };
}

#endif
