#include "ioGeometry.hpp"
#include <ioDelimiter.hpp>
#include <streamGuard.hpp>
#include <iostream>

std::istream & maslov::operator>>(std::istream & in, Point & dest)
{}

std::istream & maslov::operator>>(std::istream & in, Polygon & dest)
{}

std::ostream & maslov::operator<<(std::ostream & out, const Point & dest)
{}

std::ostream & maslov::operator<<(std::ostream & out, const Polygon & dest)
{}
