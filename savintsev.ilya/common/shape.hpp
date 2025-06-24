#ifndef SHAPE_HPP
#define SHAPE_HPP
#include <string>
#include "base-types.hpp"

namespace savintsev
{
  struct Color
  {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;

    Color() = default;
    Color(uint8_t red, uint8_t green, uint8_t blue);

    bool operator==(const Color & other) const;
  };

  class Shape
  {
  public:
    virtual ~Shape() = default;
    explicit Shape(std::string name = "");

    virtual Shape * clone() const = 0;

    virtual rectangle_t get_frame_rect() const = 0;
    virtual size_t get_all_points(point_t * ps) const = 0;

    virtual void move(point_t p) = 0;
    virtual void move(double x, double y) = 0;

    void scale(double k);
    virtual void unsafe_scale(double k) noexcept = 0;

    std::string get_name() const noexcept;
    void set_name(std::string name);

    Color get_color() const noexcept;
    void set_color(const Color & c) noexcept;
  protected:
    std::string name_;
    Color c_ = Color(255, 255, 255);
  };
}
#endif
