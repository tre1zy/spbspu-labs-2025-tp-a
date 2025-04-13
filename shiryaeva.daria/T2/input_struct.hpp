#ifndef INPUT_STRUCT_HPP
#define INPUT_STRUCT_HPP

#include <string>

namespace shiryaeva
{
	struct DelimiterIO
  {
    char exp;
  };

	struct HexUllIO
  {
    unsigned long long &ref;
  };

	struct CharIO
  {
    char &ref;
  };

	struct StringIO
  {
    std::string &ref;
  };

	std::istream &operator>>(std::istream &in, DelimiterIO &&dest);
	std::istream &operator>>(std::istream &in, HexUllIO &&dest);
	std::istream &operator>>(std::istream &in, CharIO &&dest);
	std::istream &operator>>(std::istream &in, StringIO &&dest);
}

#endif