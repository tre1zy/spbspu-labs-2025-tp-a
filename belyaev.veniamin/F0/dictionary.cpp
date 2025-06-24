#include "dictionary.hpp"
#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>

std::string belyaev::formPairString(const std::pair<const std::string, const std::string>& pair)
{
  return pair.first + " -> " + pair.second;
}

std::string belyaev::accumulatePairString(std::string result, const std::pair<const std::string, const std::string>& pair)
{
  if (result.empty())
  {
    return formPairString(pair);
  }
  return std::move(result) + "\n" + formPairString(pair);
}

std::ostream& belyaev::operator<<(std::ostream& out, const Dictionary& dictionary)
{
  if (dictionary.dict.empty())
  {
    out << "<EMPTY>\n";
    return out;
  }
  using namespace std::placeholders;
  auto accumPairBind = std::bind(accumulatePairString, _1, _2);
  std::string result = std::accumulate(dictionary.dict.begin(), dictionary.dict.end(), std::string(), accumPairBind);
  out << result << '\n';
  return out;
}