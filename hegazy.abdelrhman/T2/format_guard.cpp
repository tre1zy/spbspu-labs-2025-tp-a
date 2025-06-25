#include "format_guard.hpp"

pilugina::FormatGuard::FormatGuard(std::ostream& s) :
	stream_(s),
	flags_(s.flags()),
	precision_(s.precision()),
	fill_(s.fill())
{
}

pilugina::FormatGuard::~FormatGuard()
{
	stream_.flags(flags_);
	stream_.precision(precision_);
	stream_.fill(fill_);
}