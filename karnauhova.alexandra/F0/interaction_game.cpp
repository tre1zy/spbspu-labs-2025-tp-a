#include "interaction_game.hpp"
#include <iostream>
#include <algorithm>
#include <functional>
#include <queue>
#include "terminal_text.hpp"
using map_characters = std::map< std::string, karnauhova::Character >;
namespace
{
  void print_info(const std::pair<size_t, karnauhova::Character>& pair)
  {
    std::cout << pair.first << " ";
    pair.second.about_character();
    std::cout << "\n";
  }

  struct QueueInserter
  {
    std::queue<std::pair<std::string, karnauhova::Character>>& queue;
    void operator()(const std::pair<const std::string, karnauhova::Character>& pair) const
    {
      queue.push(pair);
    }
  };

  std::string where_damage(int option)
  {
    if (option == 1)
    {
      return "удар в голову";
    }
    else if (option == 2)
    {
      return "удар в туловище";
    }
    else if (option == 3)
    {
      return "удар в руку";
    }
    else if (option == 4)
    {
      return "удар в ногу";
    }
    return "";
  }
}

void karnauhova::choice_charact(map_characters& players, const std::map< size_t, Character >& characters)
{
  while (!std::cin.eof())
  {
    clear_screen();
    std::cout << "\033[1;33m" << std::string(100, '=') << "\033[0m" << "\n";
    std::cout << "Чтобы вернуться напишите EXIT\n\n";
    std::cout << "Напишите ник игрока, который будет выбирать персонажа:\n";
    std::string name;
    std::cin >> name;
    if (name == "EXIT")
    {
      return;
    }
    try
    {
      Character check = players.at(name);
      bool input_end = false;
      while (!input_end)
      {
        clear_screen();
        std::cout << "\033[1;33m" << std::string(100, '=') << "\033[0m" << "\n";
        std::cout << "Доступные персонажи:\n";
        std::for_each(characters.begin(), characters.end(), std::bind(print_info, std::placeholders::_1));
        std::cout << "\n" << "> GET_STORY <index> - более пожробная информация про персонажа\n";
        std::cout <<"> CHOOSE <index> - выбрать персонажа\n";
        std::string answer;
        if (!(std::cin >> answer))
        {
        if (std::cin.eof())
        {
            std::cin.clear();
            return;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        continue;
        }
        if (answer == "EXIT")
        {
          return;
        }
        else if (answer == "GET_STORY")
        {
          size_t index = 0;
          std::cin >> index;
          if (!std::cin || index == 0 || index > characters.size())
          {
            std::cin.clear();
            continue;
          }
          std::cout << characters.at(index).get_story() << "\n";
          delay(15000);
        }
        else if (answer == "CHOOSE")
        {
          size_t index = 0;
          std::cin >> index;
          if (!std::cin || index == 0 || index > characters.size())
          {
            std::cin.clear();
            continue;
          }
          players[name] = characters.at(index);
          std::cout << "\n" << characters.at(index).get_philosophy() << "\n";
          std::cout << "\n" << name << " выбрал " << players.at(name).get_name() << "\n";
          delay(2400);
          input_end = true;
        }
      }
    }
    catch (const std::exception& e)
    {
      std::cout << "Сложно ввести существующий ник?:(\n";
      delay(1200);
    }
  }
}

void karnauhova::fight(map_characters& players)
{
  std::queue< std::pair < std::string, Character > > fight_queue;
  QueueInserter inserter{fight_queue};
  std::for_each(players.begin(), players.end(), inserter);
  if (fight_queue.front().second.base_hp() == 0 || fight_queue.back().second.base_hp() == 0)
  {
    throw std::logic_error("Incorrect characters");
  }
  while (!fight_queue.front().second.is_lost())
  {
    clear_screen();
    std::cout << "\033[1;31m" << std::string(100, '=') << "\033[0m" << "\n";
    std::cout << "-Бой-\n\n";
    std::cout << "Ход " << fight_queue.front().first << "\n";
    std::cout << "Возможные действия:\n" << "> Ударить в голову(1)\n" << "> Ударить в туловище(2)\n";
    std::cout << "> Ударить в руку(3)\n" << "> Ударить в ногу(4)\n";
    std::cout << "Номер выбранного удара:\n";
    int option = 0;
    if (!(std::cin >> option) || option > 4 || option < 1)
    {
      if (std::cin.eof())
      {
        std::cin.clear();
        return;
      }
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      continue;
    }
    fight_queue.front().second.position = option;
    int protection_option = 0;
    while (!protection_option)
    {
      clear_screen();
      std::cout << "\033[1;31m" << std::string(100, '=') << "\033[0m" << "\n";
      std::cout << "-Бой-\n\n";
      std::cout << "Защищается " << fight_queue.back().first << "\n";
      std::cout << "Возможные действия:\n" << "> Защитить голову(1)\n" << "> Защитить туловище(2)\n";
      std::cout << "> Защитить руку(3)\n" << "> Защитить ногу(4)\n";
      std::cout << "Что будете защищать?(номер):\n";
      if (!(std::cin >> protection_option) || protection_option > 4 || protection_option < 1)
      {
        protection_option = 0;
        if (std::cin.eof())
        {
          std::cin.clear();
          return;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        continue;
      }
    }
    clear_screen();
    std::cout << "\033[1;31m" << std::string(100, '=') << "\033[0m" << "\n";
    std::cout << "-Бой-\n\n";
    fight_queue.push(fight_queue.front());
    fight_queue.pop();
    if (option != protection_option)
    {
      fight_queue.front().second.damage(fight_queue.back().second.attack());
      players[fight_queue.front().first].damage(fight_queue.back().second.attack());
      std::cout << fight_queue.front().first << " получает " << where_damage(option) << "\n";
      if (fight_queue.front().second.is_lost())
      {
        std::cout << "Для "<< fight_queue.front().first << " это был последний удар\n";
      }
      else
      {
        std::cout << "HP: " <<  fight_queue.front().second.volume_hp() << "\n";
      }
      delay(2500);
    }
    else
    {
      std::cout << fight_queue.front().first << " успешно защитился от атаки ";
      std::cout << fight_queue.back().first << "\n";
      delay(2500);
    }
    if (!std::cin.eof())
    {
      fight_queue.back().second.position = 0;
    }
  }
  players[fight_queue.back().first].recovery_hp();
  players[fight_queue.front().first].recovery_hp();
  clear_screen();
  std::cout << "\033[1;31m" << std::string(100, '=') << "\033[0m" << "\n";
  std::cout << fight_queue.back().first << " побеждает!\n\n";
  std::cout << fight_queue.back().first << ": " << fight_queue.back().second.get_win_quote()<<"\n\n";
  std::cout << fight_queue.front().first << ": " << fight_queue.front().second.get_lose_quote()<<"\n\n";
  std::cout << "Выход из боя...\n";
  delay(10000);
}
