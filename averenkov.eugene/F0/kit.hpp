#ifndef KIT_HPP
#define KIT_HPP

#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <stdexcept>
#include "item.hpp"

namespace averenkov
{
  class Kit
  {
  public:
    Kit(const std::string& n);

    void addItem(const Item* item);
    void removeItem(const std::string& item_name);
    bool containsItem(const std::string& item_name) const;

  private:
    std::string name_;
    std::vector< const Item* > items_;
    static bool compareItemByName(const Item* item, const std::string& name);
  };


  struct ItemNameComparer
  {
  public:
    ItemNameComparer(const std::string& n);
    bool operator()(const Item* item) const;

  private:
    const std::string& name_;

  };
}

averenkov::ItemNameComparer::ItemNameComparer(const std::string& n):
  name_(n)
{}

averenkov::Kit::Kit(const std::string& name):
  name_(name)
{}

bool averenkov::Kit::compareItemByName(const Item* item, const std::string& name)
{
  return item->getName() == name;
}

bool averenkov::ItemNameComparer::operator()(const Item* item) const
{
  return item->getName() == name_;
}


void averenkov::Kit::addItem(const Item* item)
{
  if (containsItem(item->getName()))
  {
    throw std::invalid_argument("b");
  }
  if (item != nullptr)
  {
    items_.push_back(item);
  }
}

void averenkov::Kit::removeItem(const std::string& item_name)
{
  auto it= std::remove_if(items_.begin(), items_.end(), ItemNameComparer(item_name));
  items_.erase(it, items_.end());
}

bool averenkov::Kit::containsItem(const std::string& item_name) const
{
  auto func = std::bind(compareItemByName, std::placeholders::_1, item_name);
  return std::any_of(items_.begin(), items_.end(), func);
}

#endif
