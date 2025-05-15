#include "format_guard.hpp"
#include <iostream>
#include <iomanip>
#include <string>

shiryaeva::FormatGuard::FormatGuard(std::basic_ios< char >& s) :
  s_(s),
  fill_(s.fill()),
  precision_(s.precision()),
  fmt_(s.flags())
{
}

shiryaeva::FormatGuard::~FormatGuard()
{
  s_.fill(fill_);
  s_.precision(precision_);
  s_.flags(fmt_);
}

shiryaeva::OutputFormatter::OutputFormatter(std::ostream& out):
  out_(out)
{
}

void shiryaeva::OutputFormatter::operator()(unsigned long long key1, char key2, const std::string& key3)
{
  out_ << "(:key1 0x" << std::hex << std::uppercase << key1;
  out_ << ":key2 '" << key2 << "'";
  out_ << ":key3 \"" << key3 << "\":)";
}