#ifndef ITEM_HPP
#define ITEM_HPP

#include <string>

namespace averenkov
{
  struct Item
  {
  public:
    Item(const std::string& n, int w, int v);
    std::string getName() const;
    int getWeight() const;
    int getValue() const;

  private:
    std::string name_;
    int weight_;
    int value_;
  };

}

averenkov::Item::Item(const std::string& n, int w, int v):
  name_(n),
  weight_(w),
  value_(v)
{}

std::string averenkov::Item::getName() const
{
  return name_;
}

int averenkov::Item::getWeight() const
{
  return weight_;
}

int averenkov::Item::getValue() const
{
  return value_;
}

#endif
