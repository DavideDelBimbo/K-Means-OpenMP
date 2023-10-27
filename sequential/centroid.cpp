#include "centroid.h"


namespace Sequential {
    Centroid::Centroid(const int d, const std::vector<double>& coords, const int id) : dimensions(d), coordinates(coords), clusterId(id) { }
}