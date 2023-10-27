#include "centroids.h"


namespace Parallel {
    Centroids::Centroids(const int K, const int d, const std::vector<double>& coords, const std::vector<int>& ids) : size(K), dimensions(d), coordinates(coords), clustersIds(ids) { }
}