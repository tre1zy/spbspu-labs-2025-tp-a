#include <iostream>
#include "stream_guard.hpp"

int main()
{
  rychkov::input::StreamGuard guard(std::cin);
}
