#ifndef DATA_H
#define DATA_H
#include <string>
#include <iostream>

namespace smirnov
{
    struct DataStruct
    {
	double key1;
	unsigned long long key2;
	std::string key3;
    };

    std::istream& operator>>(std::istream& in, DataStruct& value);
    std::ostream& operator<<(std::ostream& out, const DataStruct& value);

    namespace ioStructs
    {

	struct DelimiterIO
	{
		char expected;
	};

	struct DoubleIO
	{
		double& ref;
	};

	struct ULLIO
	{
		unsigned long long& ref;
	};

	struct StringIO
	{
		std::string& ref;
	};

	struct KeyNumIO
	{
		int exp;
	};

	std::istream& operator>>(std::istream& in, DoubleIO&& dest);
	std::istream& operator>>(std::istream& in, ULLIO&& dest);
	std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
	std::istream& operator>>(std::istream& in, StringIO&& dest);
	std::istream& operator>>(std::istream& in, KeyNumIO& dest);
	std::ostream& operator<<(std::ostream& out, const DoubleIO&& dest);
	std::ostream& operator<<(std::ostream& out, const ULLIO&& dest);
    }
}

#endif
