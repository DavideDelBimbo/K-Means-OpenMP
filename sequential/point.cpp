#include "point.h"


namespace Sequential {
    Point::Point(const int d, const std::vector<double>& coords, const int pId, const int cId) : dimensions(d), coordinates(coords), pointId(pId), clusterId(cId) { }
}