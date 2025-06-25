#include "characters.hpp"
#include <functional>
#include <map>
#include <random>
#include <iterator>
#include <iostream>

std::istream& karnauhova::operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c = '0';
  in >> c;
  if (in && (std::tolower(c) != dest.exp))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

karnauhova::Character::Character():
  name_("defolt"),
  hp_(0),
  total_hp_(0),
  atk1_(0),
  atk2_(0),
  story_("defolt"),
  philosophy_("defolt"),
  quote_win_("defolt"),
  quote_lose_("defolt")
{}

karnauhova::Character::Character(std::string name, int hp, int atk1, int atk2):
  name_(name),
  hp_(hp),
  total_hp_(hp),
  atk1_(atk1),
  atk2_(atk2),
  story_("defolt"),
  philosophy_("defolt"),
  quote_win_("defolt"),
  quote_lose_("defolt")
{}

int karnauhova::Character::attack()
{
    std::map< int, int > cmds;
    cmds[1] = attack_head();
    cmds[2] = attack_body();
    cmds[3] = attack_arm();
    cmds[4] = attack_leg();
    try
    {
      return cmds.at(position);
    }
    catch (...)
    {
      throw std::logic_error("Error: It's not the turn to attack");
    }
}

int karnauhova::Character::attack_head()
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(1, 10);
  int x = dist(gen);
  if (x > 8)
  {
    return atk2_;
  }
  else if (x > 5)
  {
    return atk2_ - 1;
  }
  return atk2_ - 2;
}

int karnauhova::Character::attack_body()
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(1, 10);
  int x = dist(gen);
  if (x > 9)
  {
    return atk2_ - 1;
  }
  else if (x > 5)
  {
    return atk1_ + 2;
  }
  return atk1_ + 1;
}

int karnauhova::Character::attack_arm()
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(1, 10);
  int x = dist(gen);
  if (x > 6)
  {
    return atk1_ + 1;
  }
  else if (x > 2)
  {
    return atk1_;
  }
  return 0;
}

int karnauhova::Character::attack_leg()
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(1, 10);
  int x = dist(gen);
  if (x > 7)
  {
    return atk1_ + 2;
  }
  else if (x > 3)
  {
    return atk1_ + 1;
  }
  return atk1_;
}

void karnauhova::Character::take_philosophy(std::string philosophy)
{
  philosophy_ = philosophy;
}

void  karnauhova::Character::take_story(std::string story)
{
  story_ = story;
}

void karnauhova::Character::take_quotes(std::string quote_win, std::string quote_lose)
{
  quote_win_ = quote_win;
  quote_lose_ = quote_lose;
}

bool karnauhova::Character::is_lost()
{
  return total_hp_ <= 0;
}

void karnauhova::Character::damage(int size_damage)
{
  total_hp_ -= size_damage;
}    

int karnauhova::Character::volume_hp()
{
  return total_hp_;
}

int karnauhova::Character::base_hp()
{
  return hp_;
}

std::string karnauhova::Character::get_name() const
{
  return name_;
}

std::istream& karnauhova::operator>>(std::istream& in, Character& pol)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  using del = DelimiterIO;
  std::string name;
  in >> name;
  int hp = 0, atk1 = 0, atk2 = 0;
  in >> hp >> atk1 >> del{'-'} >> atk2;
  in >> std::ws;
  std::string story, philosophy, quote_win, quote_lose;
  std::getline(in, story);
  std::getline(in, philosophy);
  std::getline(in, quote_win);
  std::getline(in, quote_lose);
  if (!in)
  {
    throw std::logic_error("Error: break file");
    return in;
  }
  Character temp(name, hp, atk1, atk2);
  temp.take_story(story);
  temp.take_philosophy(philosophy);
  temp.take_quotes(quote_win, quote_lose);
  pol = std::move(temp);
  return in;
}

void karnauhova::Character::about_character() const
{
  std::cout << name_ << " " << hp_ << " hp  " << atk1_ << "-" << atk2_ << " atk"; 
}

std::string karnauhova::Character::get_story() const
{
  return story_;
}

std::string karnauhova::Character::get_philosophy() const
{
  return philosophy_;
}
