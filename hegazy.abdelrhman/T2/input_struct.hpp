
#ifndef INPUT_STRUCT_HPP
#define INPUT_STRUCT_HPP

#include "data_struct.hpp"
#include <iostream>

namespace martynov {

	std::istream& operator>>(std::istream& is, DataStruct& ds);
	std::ostream& operator<<(std::ostream& os, const DataStruct& ds);

}  

#endif 