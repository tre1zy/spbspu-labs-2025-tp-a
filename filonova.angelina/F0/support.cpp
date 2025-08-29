#include "support.hpp"

bool filonova::is_letter(char c)
{
  return std::isalpha(static_cast< unsigned char >(c));
}

std::istream &filonova::operator>>(std::istream &in, Word &w)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  std::string temp;
  in >> temp;
  if (std::none_of(temp.begin(), temp.end(), is_letter))
  {
    in.setstate(std::ios::failbit);
  }
  else
  {
    const char *specialСhars = "'\"()";
    if (temp.find_first_of(specialСhars) != std::string::npos)
    {
      temp = temp.substr(temp.find_first_not_of(specialСhars));
      temp = temp.substr(0, temp.find_first_of(specialСhars));
    }

    while (!temp.empty() && std::ispunct(static_cast< unsigned char >(temp.back())))
    {
      temp.pop_back();
    }

    std::transform(
      temp.begin(),
      temp.end(),
      temp.begin(),
      static_cast< int (*)(int) >(std::tolower));

    w.text = std::move(temp);
  }
  return in;
}

std::ostream &filonova::operator<<(std::ostream &out, const Word &w)
{
  return out << w.text;
}

bool filonova::operator<(const Word &lhs, const Word &rhs)
{
  return lhs.text < rhs.text;
}

bool filonova::IsValidNameChar::operator()(char ch) const
{
  return std::isalnum(static_cast< unsigned char >(ch)) || ch == '_' || ch == '-';
}

bool filonova::isValidName(const std::string &name)
{
  return !name.empty() && std::all_of(name.begin(), name.end(), IsValidNameChar());
}

filonova::MergeDictEntry::MergeDictEntry(Dictionary &d): destination(d) {}

void filonova::MergeDictEntry::operator()(const std::pair< const std::string, size_t > &pair) const
{
  destination[pair.first] += pair.second;
}

filonova::CompareByFrequency::CompareByFrequency(bool desc): descending(desc) {}

bool filonova::CompareByFrequency::operator()(const std::pair< std::string, size_t > &a,
  const std::pair< std::string, size_t > &b) const
{
  if (a.second == b.second)
  {
    return a.first < b.first;
  }
  return descending ? a.second > b.second : a.second < b.second;
}

filonova::WordPresenceFilter::WordPresenceFilter(const Dictionary &d, bool state): dict2(d), presenceState(state) {}

bool filonova::WordPresenceFilter::operator()(const std::pair< const std::string, size_t > &entry) const
{
  bool found = dict2.count(entry.first);
  return presenceState ? found : !found;
}
