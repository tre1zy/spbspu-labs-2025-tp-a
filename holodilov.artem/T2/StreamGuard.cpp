#include "StreamGuard.hpp"

holodilov::StreamGuard::StreamGuard(std::basic_ios< char >& ios):
  ios_(ios),
  precision_(ios.precision()),
  fmtFlags_(ios.flags()),
  fill_(ios.fill())
{

}

holodilov::StreamGuard::~StreamGuard()
{
  ios_.precision(precision_);
  ios_.flags(fmtFlags_);
  ios_.fill(fill_);
}
