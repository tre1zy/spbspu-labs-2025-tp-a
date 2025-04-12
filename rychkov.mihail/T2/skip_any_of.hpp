#ifndef SKIP_ANY_OF_HPP
#define SKIP_ANY_OF_HPP

#include <istream>
#include <algorithm>
#include "stream_guard.hpp"

namespace rychkov
{
  namespace input
  {
    template< size_t... Lens >
    struct match_any
    {
      const char* cases[sizeof...(Lens)];
      size_t* result_match;
      char* buffer;
      match_any(const char(&...strings)[Lens], size_t* resultMatch = nullptr, char* buf = nullptr):
        cases{strings...},
        result_match(resultMatch),
        buffer(buf)
      {}
    };
    template< size_t... Lens >
    std::istream& operator>>(std::istream& in, const match_any< Lens... >& possible)
    {
      constexpr size_t minLen = std::min({Lens...});
      constexpr size_t maxLen = std::max({Lens...});
      if (minLen <= 1)
      {
        return in;
      }
      std::istream::sentry sentry(in);
      if (!sentry)
      {
        return in;
      }
      StreamGuard guard(in);

      char static_buffer[maxLen]{};
      char* buffer = (possible.buffer == nullptr ? static_buffer : possible.buffer);
      bool skips[sizeof...(Lens)]{};
      constexpr size_t sizes[] = {(Lens - 1)...};
      bool atleastOne = true;
      for (size_t i = 0; atleastOne && (i + 1 < maxLen) && (in >> buffer[i] >> std::noskipws); i++)
      {
        atleastOne = false;
        for (size_t j = 0; j < sizeof...(Lens); j++)
        {
          if (!skips[j])
          {
            if ((i >= sizes[j]) || (buffer[i] != possible.cases[j][i]))
            {
              skips[j] = true;
              continue;
            }
            else if (i + 1 == sizes[j])
            {
              if (possible.result_match != nullptr)
              {
                *possible.result_match = j;
              }
              return in;
            }
            atleastOne = true;
          }
        }
      }
      if (possible.result_match != nullptr)
      {
        *possible.result_match = -1;
      }
      in.setstate(std::ios::failbit);
      return in;
    }

    template< size_t... Lens >
    match_any< Lens... > anyOf(const char(&...strings)[Lens])
    {
      return {strings...};
    }
    template< size_t... Lens >
    match_any< Lens... > anyOf(size_t* resultMatch, const char(&...strings)[Lens])
    {
      return {strings..., resultMatch};
    }
    template< size_t... Lens >
    match_any< Lens... > anyOf(size_t* resultMatch, char* buf, const char(&...strings)[Lens])
    {
      return {strings..., resultMatch, buf};
    }
  }
}

#endif
