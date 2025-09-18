#include "exceptions.hpp"

const char* holodilov::InvalidCommandException::what() const noexcept
{
  return "Error: invalid command.\n";
}

const char* holodilov::DictionaryNotFoundException::what() const noexcept
{
  return "Error: dictionary not found.\n";
}

const char* holodilov::DictionaryExportException::what() const noexcept
{
  return "Error: unable to write dictionary into file.\n";
}

const char* holodilov::DictionaryAlreadyExistsException::what() const noexcept
{
  return "Error: dictionary with this name already exists.\n";
}

const char* holodilov::WordNotFoundException::what() const noexcept
{
  return "Error: word not found in this dictionary.\n";
}

const char* holodilov::DictionaryFileNotFoundException::what() const noexcept
{
  return "Error: dictionary file not found.\n";
}

const char* holodilov::InvalidDictionaryFileException::what() const noexcept
{
  return "Error: dictionary file has invalid format.\n";
}

const char* holodilov::AlphabetExportException::what() const noexcept
{
  return "Error: unable to create alphabet file.\n";
}

const char* holodilov::AlphabetFileNotFoundException::what() const noexcept
{
  return "Error: alphabet file not found.\n";
}

const char* holodilov::InvalidAlphabetFileException::what() const noexcept
{
  return "Error: unable to read alphabet file.\n";
}