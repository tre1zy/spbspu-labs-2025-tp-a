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
    Character();
    Character(std::string name, int hp, int atk1, int atk2);
    bool operator==(const Character& other) const;
    int position;
    int attack();
    void take_philosophy(std::string philosophy);
    void take_story(std::string story);
    void take_quotes(std::string quote_win, std::string quote_lose);
    bool is_lost();
    void damage(int size_damage);
    int volume_hp() const;
    int base_hp() const;
    std::string get_name() const;
    std::string get_story() const;
    std::string get_atk() const;
    std::string get_philosophy() const;
    std::string get_win_quote() const;
    std::string get_lose_quote() const;
    void about_character() const;
    void recovery_hp();

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
  std::ostream& operator<<(std::ostream& out, const Character& character);
  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
}
#endif
