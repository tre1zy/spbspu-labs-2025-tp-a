#ifndef FORMAT_GUARD_HPP
#define FORMAT_GUARD_HPP

#include <ios>

namespace fedorov {
class FormatGuard {
 public:
  explicit FormatGuard(std::ios& stream);
  ~FormatGuard();

 private:
  std::ios& stream_;
  std::ios::fmtflags flags_;
  char fill_;
  std::streamsize precision_;
};
} // namespace fedorov

#endif
