#ifndef FORMAT_GUARD_HPP
#define FORMAT_GUARD_HPP

#include <ios>

namespace shiryaeva
{
  class FormatGuard
  {
  public:
    explicit FormatGuard(std::basic_ios< char > &s);
    ~FormatGuard();

    FormatGuard(const FormatGuard &) = delete;
    FormatGuard &operator=(const FormatGuard &) = delete;
    FormatGuard(FormatGuard &&) = delete;
    FormatGuard &operator=(FormatGuard &&) = delete;

  private:
    std::basic_ios< char > &s_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
  };

  class OutputFormatter
  {
  public:
    explicit OutputFormatter(std::ostream& out);
    void operator()(unsigned long long key1, char key2, const std::string& key3);

  private:
    std::ostream& out_;
  };
}

#endif
