#ifndef PROCESSORS_HPP
#define PROCESSORS_HPP

#include "parser.hpp"
#include "polygon.hpp"

namespace rychkov
{
  class MainProcessor
  {
  public:
    bool init(ParserContext& context, int argc, char** argv);
    bool area(ParserContext& context);
    bool max(ParserContext& context);
    bool min(ParserContext& context);
    bool count(ParserContext& context);
    bool remove_repeates(ParserContext& context);
    bool rectangles(ParserContext& context);
  private:
    std::vector< Polygon > polygons_;
  };
  struct AreaProcessor
  {
    std::vector< Polygon >& polygons;

    bool even(ParserContext& context);
    bool odd(ParserContext& context);
    bool mean(ParserContext& context);
    bool count(ParserContext& context);
  };
  struct MaxProcessor
  {
    std::vector< Polygon >& polygons;

    bool area(ParserContext& context);
    bool count(ParserContext& context);
  };
  struct MinProcessor
  {
    std::vector< Polygon >& polygons;

    bool area(ParserContext& context);
    bool count(ParserContext& context);
  };
  struct CountProcessor
  {
    std::vector< Polygon >& polygons;

    bool even(ParserContext& context);
    bool odd(ParserContext& context);
    bool count(ParserContext& context);
  };
  size_t parse_as_count(ParserContext& context);
  struct get_area
  {
    double operator()(const Polygon& polygon);
  };
}

#endif
