#include "centroids.h"


namespace Parallel {
    Centroids::Centroids(const int K, const int d, double* coords, int* ids) : size(K), dimensions(d), coordinates(coords), clustersIds(ids) { }

    Centroids::~Centroids() {
        delete[] coordinates;
        delete[] clustersIds;
    }
}