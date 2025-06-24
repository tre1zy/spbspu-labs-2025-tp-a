#ifndef CHARACTERS_HPP
#define CHARACTERS_HPP

#include <string>

namespace karnauhova
{
    struct DelimiterIO
  {
    char exp;
  };

  struct Character
  {
  public:
    Character(std::string name, int hp, int atk1, int atk2);
    int position;
    int attack();
    void take_philosophy(std::string philosophy);
    void take_story(std::string story);
    void take_quotes(std::string quote_win, std::string quote_lose);
    bool is_lost();
    void damage(int size_damage);
    int volume_hp();
    int base_hp();

  private:
    std::string name_;
    int hp_;
    int total_hp_;
    int atk1_;
    int atk2_;
    std::string story_;
    std::string philosophy_;
    std::string quote_win_;
    std::string quote_lose_;

    int attack_head();
    int attack_body();
    int attack_arm();
    int attack_leg();
  };
  std::istream& operator>>(std::istream& in, Character& pol);
}
#endif
