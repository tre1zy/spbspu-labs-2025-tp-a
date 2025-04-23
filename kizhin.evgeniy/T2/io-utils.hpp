#ifndef SPBSPU_LABS_2025_TP_A_KIZHIN_EVGENIY_T2_IO_UTILS_HPP
#define SPBSPU_LABS_2025_TP_A_KIZHIN_EVGENIY_T2_IO_UTILS_HPP

#include <iosfwd>
#include "data-struct.hpp"

namespace kizhin {
  std::istream& operator>>(std::istream&, DataStruct&);
  std::ostream& operator<<(std::ostream&, const DataStruct&);
}

#endif

