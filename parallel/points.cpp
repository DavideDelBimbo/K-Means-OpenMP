#include "points.h"


namespace Parallel {
    Points::Points(const int N, const int d, const std::vector<double>& coords, const std::vector<int>& ids) : size(N), dimensions(d), coordinates(coords), pointsIds(ids), clustersIds(N, -1) { }
}