#include "data_struct.hpp"
#include <iostream>
#include <sstream>
#include "io_fmt_guard.hpp"

namespace {
  struct DoubleIO
  {
    double& ref_;
  };

  struct UllIO
  {
    unsigned long long& ref_;
  };

  struct StringIO
  {
    std::string& ref_;
  };

  struct DelimiterIO
  {
    char exp_;
  };

  struct LabelIO
  {
    std::string exp_;
  };

  std::istream& operator>>(std::istream& in, DoubleIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    std::string n = "";
    std::getline(in, n, ':');
    std::istringstream iss(n);
    if (!(iss >> dest.ref_)) {
      in.setstate(std::ios::failbit);
    }
  }

  std::istream& operator>>(std::istream& in, UllIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    std::string n = "";
    std::getline(in, n, ':');
    std::istringstream iss(n);
    if (!(iss >> dest.ref_)) {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, StringIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    return std::getline(in >> DelimiterIO{'"'}, dest.ref_, '"');
  }

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    char c = '0';
    in >> c;
    if (in && (c != dest.exp_)) {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, LabelIO& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    std::string data = "";
    if (in >> StringIO{data}) {
      in.setstate(std::ios::failbit);
    }
    dest.exp_ = data;
    return in;
  }
}

std::istream& maslevtsov::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  DataStruct result;
  in >> DelimiterIO{'('} >> DelimiterIO{':'};
  for (std::size_t i = 0; i < 3ull; ++i) {
    LabelIO label{"0"};
    in >> label;
    if (label.exp_ == "key1") {
      in >> DoubleIO{result.key1_} >> DelimiterIO{':'};
    } else if (label.exp_ == "key2") {
      in >> UllIO{result.key2_} >> DelimiterIO{':'};
    } else if (label.exp_ == "key3") {
      in >> StringIO{result.key3_};
    } else {
      in.setstate(std::ios::failbit);
    }
  }
  in >> DelimiterIO{':'} >> DelimiterIO{')'};
  if (in) {
    dest = result;
  }
  return in;
}

std::ostream& maslevtsov::operator<<(std::ostream& out, const DataStruct& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry) {
    return out;
  }
  IOFmtGuard fmtguard(out);
  out << "(:key1 " << dest.key1_ << ":";
  out << "key2_ " << dest.key2_ << "ull:";
  out << "key3_ \"" << dest.key3_ << "\":)";
  return out;
}
