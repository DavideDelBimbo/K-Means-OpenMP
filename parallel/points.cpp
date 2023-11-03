#include "points.h"


namespace Parallel {
    Points::Points(const int N, const int d, double* coords, int* pIds, int* cIds) : size(N), dimensions(d), coordinates(coords), pointsIds(pIds), clustersIds(cIds) { }

    Points::~Points() {
        delete[] coordinates;
        delete[] pointsIds;
        delete[] clustersIds;
    }
}