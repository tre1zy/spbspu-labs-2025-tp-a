#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include "keys.hpp"

int main()
{
  using namespace alymova;
  std::ostream& out = std::cout;
  std::istream& in = std::cin;
  StreamGuard os_sg(out);
  StreamGuard is_sg(in);
  DataStruct d;
  in >> d;
  out << d << '\n';
  /*std::istringstream iss("(:key1 076:key2 'c':key3 \"Let madness release you\":)");
  std::copy(
    std::istream_iterator< DataStruct >(iss >> d),
    std::istream_iterator< DataStruct >(),
    std::back_inserter(data)
  );

  std::cout << "Data:\n";
  std::copy(
    std::begin(data),
    std::end(data),
    std::ostream_iterator< DataStruct >(std::cout, "\n")
  );*/
}
