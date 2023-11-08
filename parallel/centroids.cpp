#include "centroids.h"


namespace Parallel {
    Centroids::Centroids(const int k, const int d, double* coords, int* ids) : size(k), dimensions(d), coordinates(coords), clustersIds(ids) { }

    Centroids::~Centroids() {
        delete[] coordinates;
        delete[] clustersIds;
    }
}