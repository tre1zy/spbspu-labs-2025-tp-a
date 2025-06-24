#include <iostream>
#include <map>
#include <fstream>
#include "characters.hpp"
#include "input_file.hpp"

int main()
{
  setlocale(LC_ALL, "ru");
  using namespace karnauhova;
  std::map< size_t, Character > characters;
  std::map< std::string, Character > players;
  players = input_data(characters);
  if (players.size() != 0)
  {
    auto it = players.begin();
    if (it->second.position != 0 || (++it)->second.position != 0)
    {
      std::cout << "\n";
    }
    std::cout << "С возвращением " << it->first << ", " << (++it)->first << "\n";
  }
  auto it = players.begin();
  std::cout << it->second.get_name();
  //считать данные
  //std::cout << imp.attack();
  //while (!std::cin.eof())
  //{
  //}
  //сохранить данные
}
