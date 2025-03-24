#include <string>

namespace demehin
{
  struct DataStruct
  {
    double key1;
    long long key2;
    std::string key3;
  };

  std::ostream& operator<<(std::ostream& out, const DataStruct& dest);
  std::istream& operator>>(std::istream& in, DataStruct& dest);

  namespace ioStructs
  {
    struct DoubleIO
    {
      double& ref;
    }

    struct LlIO
    {
      long long& ref;
    }

    struct DelimiterIO
    {
      char exp;
    }

    struct LabelIO
    {
      std::string exp;
    }

    struct StringIO
    {
      std::string& ref;
    }

    std::istream& operator>>(std::istream& in, DoubleIO&& dest);
    std::istream& operator>>(std::istream& in, LlIO&& dest);
    std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
    std::istream& operator>>(std::istream& in, LabelIO&& dest);
    std::istream& operator>>(std::istream& in, LabelIO&& dest);

  }
}
