#ifndef SPBSPU_LABS_2025_TP_A_KIZHIN_EVGENIY_T2_STREAM_GUARD_HPP
#define SPBSPU_LABS_2025_TP_A_KIZHIN_EVGENIY_T2_STREAM_GUARD_HPP

#include <ios>

namespace kizhin {
  class StreamGuard;
}

class kizhin::StreamGuard
{
public:
  StreamGuard(std::basic_ios< char >&);
  ~StreamGuard();

private:
  std::basic_ios< char >& s_;
  char fill_;
  std::streamsize precision_;
  std::streamsize width_;
  std::basic_ios< char >::fmtflags flags_;
};

#endif

