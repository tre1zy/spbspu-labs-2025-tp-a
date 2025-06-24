#include <iostream>
#include <map>
#include "characters.hpp"
#include "input_file.hpp"

int main()
{
  using namespace karnauhova; 
  std::map< size_t, Character > characters;
  std::map< size_t, Character > players;
  input_data(characters);
  Character imp("Meow", 50, 5, 10);
  imp.position = 1;
  //считать данные
  //std::cout << imp.attack();
  //while (!std::cin.eof())
  //{
  //}
  //сохранить данные
}
