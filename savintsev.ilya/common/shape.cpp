#include "shape.hpp"
#include <stdexcept>

savintsev::Color::Color(uint8_t red, uint8_t green, uint8_t blue):
  r(red),
  g(green),
  b(blue)
{}

bool savintsev::Color::operator==(const Color & other) const
{
  return r == other.r && g == other.g && b == other.b;
}

savintsev::Shape::Shape(std::string name):
  name_(name)
{}

void savintsev::Shape::scale(double k)
{
  if (k <= 0)
  {
    throw std::invalid_argument("invalid ratio");
  }
  unsafe_scale(k);
}

std::string savintsev::Shape::get_name() const noexcept
{
  return name_;
}

void savintsev::Shape::set_name(std::string name)
{
  name_ = name;
}

savintsev::Color savintsev::Shape::get_color() const noexcept
{
  return c_;
}

void savintsev::Shape::set_color(const Color & c) noexcept
{
  c_ = c;
}
