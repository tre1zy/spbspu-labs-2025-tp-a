#include "DataStruct.hpp"
#include <iomanip>
#include "IOStreamGuard.hpp"

namespace gavrilova {

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    char c;
    in >> c;
    if (in && c != dest.exp) {
        in.setstate(std::ios::failbit);
    }
    // if (!in) {
    //   std::cout << "Fail in delimetr";
    // }
    // else
    // {
    //   std::cout << "Good in delimetr";
    // }
    return in;
  }

  std::istream& operator>>(std::istream& in, SymbolIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    char c;
    in >> c;
    if (in && (std::tolower(c) != std::tolower(dest.exp))) {
        in.setstate(std::ios::failbit);
    }
    // if (!in) {
    //   std::cout << "Fail in symbol";
    // }
    // else
    // {
    //   std::cout << "Good in symbol";
    // }
    return in;
  }

  std::istream& operator>>(std::istream& in, DoubleIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    
    in >> dest.ref >> SymbolIO{'d'};

    // if (!in) {
    //   std::cout << "Fail in double";
    // }
    // else
    // {
    //   std::cout << "Good in double";
    // }
    return in;
  }

  std::istream& operator>>(std::istream& in, LongLongIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    in >> dest.ref >> SymbolIO{'l'} >> SymbolIO{'l'};
    // if (!in) {
    //   std::cout << "Fail in long";
    // }
    // else
    // {
    //   std::cout << "Good in long";
    // }
    return in;
  }

  std::istream& operator>>(std::istream& in, String_with_quotsIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
    // if (!in) {
    //   std::cout << "Fail in striing";
    // }
    // else
    // {
    //   std::cout << "Good in string";
    // }
    return in;
  }

  std::istream& operator>>(std::istream& in, StringI_with_spaceIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    std::getline(in, dest.ref, ' ');
    // if (!in) {
    //   std::cout << "Fail in striing";
    // }
    // else
    // {
    //   std::cout << "Good in string";
    // }
    return in;
  }

  std::istream& operator>>(std::istream& in, LabelIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    std::string data;
    if ((in >> StringI_with_spaceIO{ data }) && (data != dest.exp)) {
      in.setstate(std::ios::failbit);
    }
    // std::cout << "\n" << data << " " << "dest.exp";
    // if (!in) {
    //   std::cout << "Fail in lable";
    // }
    // else
    // {
    //   std::cout << "Good in lable";
    // }
    return in;
  }

  std::istream& operator>>(std::istream& in, DataStruct& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }

    DataStruct input;
    {
      using sep = DelimiterIO;
      using label = LabelIO;
      using ll = LongLongIO;
      using dbl = DoubleIO;
      using str = String_with_quotsIO;

      in >> sep{ '('};
      in >> sep{ ':' };
      in >> label{ "key1" } >> dbl{ input.key1 };
      in >> sep{ ':' };
      in >> label{ "key2" } >> ll{ input.key2 };
      in >> sep{ ':' };
      in >> label{ "key3" } >> str{ input.key3 };
      in >> sep{ ':' };
      in >> sep{ ')' };
    }

    if (in) {
      dest = input;
    }
    return in;
  }

  std::ostream& operator<<(std::ostream& out, const DataStruct& src) {
      std::ostream::sentry sentry(out);
      if (!sentry) {
          return out;
      }
      IOStreamGuard fmtguard(out);
      out << "(:key1 " << src.key1 << "ull:key2 " << src.key2 << ":" << "key3 \"" << src.key3 << "\":)";
      return out;
  }

}
