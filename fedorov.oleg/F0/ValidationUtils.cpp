#include "ValidationUtils.hpp"
#include "HashTable.hpp"

namespace crossref
{

  namespace validation
  {

    void checkArgCount(const std::vector< std::string > &args, size_t expected, const std::string &usage)
    {
      if (args.size() < expected)
      {
        throw std::runtime_error("Usage: " + usage);
      }
    }

    template < typename Container >
    void checkIdExists(const Container &container, const std::string &id, const std::string &error)
    {
      if (container.find(id) != container.end())
      {
        throw std::runtime_error(error);
      }
    }

    template < typename Container >
    void checkIdNotFound(const Container &container, const std::string &id, const std::string &error)
    {
      if (container.find(id) == container.end())
      {
        throw std::runtime_error(error);
      }
    }

    void checkLineRange(int start, int end, size_t max_lines)
    {
      auto start_cast = static_cast< size_t >(start);
      auto end_cast = static_cast< size_t >(start);
      if (start < 1 || end < 1 || start > end || start_cast > max_lines || end_cast > max_lines)
      {
        throw std::runtime_error("<INVALID RANGE>");
      }
    }

    void checkPositive(int value, const std::string &param)
    {
      if (value <= 0)
      {
        throw std::runtime_error("<INVALID " + param + ">");
      }
    }

    template void checkIdExists< std::map< std::string, std::vector< std::string > > >(
        const std::map< std::string, std::vector< std::string > > &, const std::string &, const std::string &);

    template void checkIdNotFound< std::map< std::string, std::vector< std::string > > >(
        const std::map< std::string, std::vector< std::string > > &, const std::string &, const std::string &);

    template void checkIdExists< std::map< std::string, HashTable > >(const std::map< std::string, HashTable > &,
                                                                      const std::string &,
                                                                      const std::string &);

    template void checkIdNotFound< std::map< std::string, HashTable > >(const std::map< std::string, HashTable > &,
                                                                        const std::string &,
                                                                        const std::string &);
  }

}
