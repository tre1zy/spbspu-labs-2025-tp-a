#include "data_struct.hpp"
#include <cmath>
#include "io_fmt_guard.hpp"

namespace {
  struct DelimiterIO
  {
    char exp_;
  };

  struct LabelIO
  {
    std::string exp_;
  };

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
    if (!std::getline(in, data, ' ')) {
      in.setstate(std::ios::failbit);
    }
    dest.exp_ = data;
    return in;
  }

  std::istream& operator>>(std::istream& in, DoubleIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    std::string n = "";
    std::getline(in, n, ':');
    if (n.find("e") == n.npos && n.find("E") == n.npos) {
      in.setstate(std::ios::failbit);
    }
    try {
      dest.ref_ = std::stod(n);
    } catch (const std::exception& e) {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, UllIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    std::string n = "";
    std::getline(in, n, ':');
    if (n.find("ull") == n.npos && n.find("ULL") == n.npos) {
      in.setstate(std::ios::failbit);
    }
    try {
      dest.ref_ = std::stoull(n);
    } catch (const std::exception& e) {
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
    LabelIO label{""};
    in >> label;
    if (label.exp_ == "key1") {
      in >> DoubleIO{result.key1_};
    } else if (label.exp_ == "key2") {
      in >> UllIO{result.key2_};
    } else if (label.exp_ == "key3") {
      in >> StringIO{result.key3_};
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
  out << "(:key1 ";
  if (dest.key1_ == 0.0) {
    out << "0.0e0";
  } else {
    int exponent = static_cast< int >(std::floor(std::log10(std::abs(dest.key1_))));
    double mantissa = dest.key1_ / std::pow(10.0, exponent);
    if (mantissa >= 10.0) {
      mantissa /= 10.0;
      exponent += 1;
    } else if (mantissa < 1.0) {
      mantissa *= 10.0;
      exponent -= 1;
    }
    out << std::fixed << std::setprecision(1);
    if (exponent > 0) {
      out << mantissa << 'e' << '+' << exponent;
    } else {
      out << mantissa << 'e' << exponent;
    }
  }
  out << ":key2 " << dest.key2_ << "ull:";
  out << "key3 \"" << dest.key3_ << "\":)";
  return out;
}
