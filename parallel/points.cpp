#include "points.h"


namespace Parallel {
    Points::Points(const int n, const int d, double* coords, int* pIds, int* cIds) : size(n), dimensions(d), coordinates(coords), pointsIds(pIds), clustersIds(cIds) { }

    Points::~Points() {
        delete[] coordinates;
        delete[] pointsIds;
        delete[] clustersIds;
    }
}