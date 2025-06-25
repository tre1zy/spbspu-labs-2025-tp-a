#include "terminal_text.hpp"
#include <iostream>

void karnauhova::clear_screen()
{
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}

 void karnauhova::hello_world(std::map< std::string, Character >& players)
 {
  clear_screen();
  std::cout << "\033[1;32m" << std::string(100, '=') << "\033[0m" << "\n";
  std::cout << "Игрок 1, введите свой ник(в одно слово):\n";
  std::string name1, name2;
  std::cin >> name1;
  players[name1] = Character();
  std::cout << "Игрок 2, введите свой ник(все также в одно слово и чтобы не повторялся c " << name1 << "):\n";
  std::cin >> name2;
  if (name1 == name2)
  {
    std::cout << "Ну если по нормальному нельзя, то бб\n";
    throw std::logic_error("debils in play");
  }
  std::cout << "\n" << "Приятно познакомиться " <<  name1 << " и " << name2 << " ^^\n";
 }