#include "point.h"


namespace Sequential {
    Point::Point(const int d, const std::vector<double>& coords, const int id) : dimensions(d), coordinates(coords), pointId(id), clusterId(-1) { }
}