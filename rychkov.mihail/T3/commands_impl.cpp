#include "processors.hpp"

#include <iostream>
#include <algorithm>
#include <numeric>
#include <functional>
#include <type_traits>

namespace rychkov
{
  static auto get_vertexes = std::bind(std::mem_fn(&Polygon::vertexes), std::placeholders::_1);
  static auto get_size = std::bind(&decltype(Polygon::vertexes)::size, get_vertexes);
  static auto accumulator = std::bind(std::plus<>{},
        std::placeholders::_1, std::bind(get_area{}, std::placeholders::_2));
}

bool rychkov::AreaProcessor::even(ParserContext& context)
{
  if (!context.eol())
  {
    return false;
  }
  using namespace std::placeholders;
  using polygons_container = std::remove_reference_t< decltype(polygons) >;
  auto iseven = std::bind(std::equal_to<>{}, std::bind(std::modulus<>{}, get_size, 2), 0);
  polygons_container::iterator workspace_end = std::partition(polygons.begin(), polygons.end(), iseven);
  context.out << std::accumulate(polygons.begin(), workspace_end, 0.0, accumulator) << '\n';
  return true;
}
bool rychkov::AreaProcessor::odd(ParserContext& context)
{
  if (!context.eol())
  {
    return false;
  }
  using namespace std::placeholders;
  using polygons_container = std::remove_reference_t< decltype(polygons) >;
  auto isodd = std::bind(std::equal_to<>{}, std::bind(std::modulus<>{}, get_size, 2), 1);
  polygons_container::iterator workspace_end = std::partition(polygons.begin(), polygons.end(), isodd);
  context.out << std::accumulate(polygons.begin(), workspace_end, 0.0, accumulator) << '\n';
  return true;
}
bool rychkov::AreaProcessor::count(ParserContext& context)
{
  size_t number = parse_as_count(context);
  if (number == -1)
  {
    return false;
  }
  if (!context.eol())
  {
    context.parse_error();
    return true;
  }
  using polygons_container = std::remove_reference_t< decltype(polygons) >;
  auto equal_size = std::bind(std::equal_to<>{}, number, std::bind(get_size, std::placeholders::_1));
  polygons_container::iterator workspace_end = std::partition(polygons.begin(), polygons.end(), equal_size);
  context.out << std::accumulate(polygons.begin(), workspace_end, 0.0, accumulator) << '\n';
  return true;
}
bool rychkov::AreaProcessor::mean(ParserContext& context)
{
  if (!context.eol())
  {
    return false;
  }
  context.out << std::accumulate(polygons.begin(), polygons.end(), 0.0, accumulator) << '\n';
  return true;
}

bool rychkov::CountProcessor::even(ParserContext& context)
{
  if (!context.eol())
  {
    return false;
  }
  auto iseven = std::bind(std::equal_to<>{}, std::bind(std::modulus<>{}, get_size, 2), 0);
  context.out << std::count_if(polygons.begin(), polygons.end(), iseven) << '\n';
  return true;
}
bool rychkov::CountProcessor::odd(ParserContext& context)
{
  if (!context.eol())
  {
    return false;
  }
  auto isodd = std::bind(std::equal_to<>{}, std::bind(std::modulus<>{}, get_size, 2), 1);
  context.out << std::count_if(polygons.begin(), polygons.end(), isodd) << '\n';
  return true;
}
bool rychkov::CountProcessor::count(ParserContext& context)
{
  size_t number = parse_as_count(context);
  if (number == -1)
  {
    return false;
  }
  if (!context.eol())
  {
    context.parse_error();
    return true;
  }
  auto equal_size = std::bind(std::equal_to<>{}, number, std::bind(get_size, std::placeholders::_1));
  context.out << std::count_if(polygons.begin(), polygons.end(), equal_size) << '\n';
  return true;
}

bool rychkov::MaxProcessor::area(ParserContext& context)
{
  if (!context.eol())
  {
    return false;
  }
  using namespace std::placeholders;
  get_area calc;
  auto compare = std::bind(std::less<>{}, std::bind(calc, _1), std::bind(calc, _2));
  context.out << calc(*std::max_element(polygons.begin(), polygons.end(), compare)) << '\n';
  return true;
}
bool rychkov::MaxProcessor::count(ParserContext& context)
{
  if (!context.eol())
  {
    return false;
  }
  using namespace std::placeholders;
  auto compare = std::bind(std::less<>{}, std::bind(get_size, _1), std::bind(get_size, _2));
  context.out << get_size(*std::max_element(polygons.begin(), polygons.end(), compare)) << '\n';
  return true;
}
bool rychkov::MinProcessor::area(ParserContext& context)
{
  if (!context.eol())
  {
    return false;
  }
  using namespace std::placeholders;
  get_area calc;
  auto compare = std::bind(std::less<>{}, std::bind(calc, _1), std::bind(calc, _2));
  context.out << calc(*std::min_element(polygons.begin(), polygons.end(), compare)) << '\n';
  return true;
}
bool rychkov::MinProcessor::count(ParserContext& context)
{
  if (!context.eol())
  {
    return false;
  }
  using namespace std::placeholders;
  auto compare = std::bind(std::less<>{}, std::bind(get_size, _1), std::bind(get_size, _2));
  context.out << get_size(*std::min_element(polygons.begin(), polygons.end(), compare)) << '\n';
  return true;
}

bool rychkov::MainProcessor::remove_repeates(ParserContext& context)
{
  if (!context.eol())
  {
    return false;
  }
  return true;
}
bool rychkov::MainProcessor::rectangles(ParserContext& context)
{
  if (!context.eol())
  {
    return false;
  }
  struct is_rect
  {
    bool operator()(const Polygon& p)
    {
      bool horis1 = (p.vertexes[0].x == p.vertexes[1].x) && (p.vertexes[2].x == p.vertexes[3].x);
      bool vert1 = (p.vertexes[0].y == p.vertexes[1].y) && (p.vertexes[2].y == p.vertexes[3].y);
      bool horis2 = (p.vertexes[1].x == p.vertexes[2].x) && (p.vertexes[3].x == p.vertexes[0].x);
      bool vert2 = (p.vertexes[1].y == p.vertexes[2].y) && (p.vertexes[3].y == p.vertexes[0].y);
      return (horis1 && vert1) || (horis2 && vert2);
    }
  };
  context.out << std::count_if(polygons_.begin(), polygons_.end(), is_rect{}) << '\n';
  return true;
}
double rychkov::get_area::operator()(const Polygon& polygon)
{
  return 0;
}
