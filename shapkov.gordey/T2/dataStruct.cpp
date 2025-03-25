#include "dataStruct.hpp"

/*std::istream& shapkov::operator>>(std::istream& in, DataStruct& rhs)
{

}*/

std::ostream& shapkov::operator<<(std::ostream& out, DataStruct& rhs)
{
  out << "(:key1 ";
  out << rhs.key1.key;
  out << ":key2 (:N ";
  out << rhs.key2.key.first;
  out << ":D ";
  out << rhs.key2.key.second;
  out << ":):key3 \"";
  out << rhs.key3.key << "\":)";
  return out;
}
