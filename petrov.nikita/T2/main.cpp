#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>
#include "data_struct.hpp"

namespace petrov
{
  void sort(std::vector< DataStruct > & vector);
}

int main()
{
  using namespace petrov;
  std::vector< DataStruct > data;
  while (!std::cin.eof())
  {
    std::copy(
    std::istream_iterator< DataStruct >(std::cin),
    std::istream_iterator< DataStruct >(),
    std::back_inserter(data)
    );
    std::cin.clear();
    std::cin.ignore(1024, '\n');
  }
  petrov::sort(data);
  std::copy(
      std::begin(data),
      std::end(data),
      std::ostream_iterator< DataStruct >(std::cout, "\n")
  );
}

void petrov::sort(std::vector< DataStruct > & vector)
{
  bool swapped = false;
  for (size_t k = 1; k <= vector.size(); k++)
  {
    if (vector[k].key1 != vector [k - 1].key1)
    {
      for (size_t i = 0; i < vector.size(); i++) 
      {
        swapped = false;
        for (size_t j = 0; j < vector.size() - i; j++) 
        {
          if (vector[j].key1 > vector[j + 1].key2) 
          {
            std::swap(vector[j], vector[j + 1]);
            swapped = true;
          }
        }
        if (!swapped)
        {
          break;
        }
      }
      return;
    }
  }
  for (size_t i = 1; i <= vector.size(); i++)
  {
    if (vector[i].key2 != vector [i - 1].key2)
    {
      return;
    }
  }
  for (size_t i = 1; i <= vector.size(); i++)
  {
    if (vector[i].key3.length() != vector [i - 1].key3.length())
    {
      return;
    }
  }
}