#include <iostream>

#include "centroid.h"


Centroid::Centroid(double x, double y) : x(x), y(y) {}

std::ostream &operator<<(std::ostream &os, const Centroid &c) {
    return os << "(" << c.x << ", " << c.y << ")";
}