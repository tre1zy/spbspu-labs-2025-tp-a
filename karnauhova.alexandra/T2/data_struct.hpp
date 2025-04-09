#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP
#include <string>

namespace karnauhova
{
  struct DataStruct
  {
    double key1;
    unsigned long long key2;
    std::string key3;    
  };

  struct UllIO
  {
    unsigned long long& ref;
  };
  
  struct DelimiterIO
  {
    char exp;
  };

  struct DoubleIO
  {
    double& ref;
  };

  struct StringIO
  {
    std::string& ref;
  };

  struct LabelIO
  {
    std::string exp;
  };
}
#endif
