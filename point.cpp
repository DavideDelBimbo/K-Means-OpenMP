#include <iostream>

#include "point.h"


Point::Point(double x, double y) : x(x), y(y), cluster(-1) {}

std::ostream &operator<<(std::ostream &os, const Point &p) {
    return os << "(" << p.x << ", " << p.y << ")";
}