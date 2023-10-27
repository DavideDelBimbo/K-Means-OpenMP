#ifndef K_MEANS_PARALLEL_CENTROID_H
#define K_MEANS_PARALLEL_CENTROID_H

#include <ostream>
#include <vector>


namespace Parallel {
  // Centroids in multidimensional space using SoA format.
  struct Centroids {
    int size; // Number of clusters.
    int dimensions; // Number of dimensions.

    std::vector<double> coordinates; // Vector for coordinates of all dimensions (x1, x2, x3, ..., y1, y2, y3, ..., z1, z2, z3, ...).
    std::vector<int> clustersIds; // Identifiers of the clusters.


    /*
      * Centroids constructor.
      * 
      * @param size: Number of clusters.
      * @param dimensions: Number of dimensions.
      * @param coordinates: Vectors of coordinates of the centroids.
      * @param centroidsIds: Identifiers of the centroid.
    */
    Centroids(const int size, const int dimensions, const std::vector<double>& coordinates, const std::vector<int>& centroidsIds);
  };
}

#endif // K_MEANS_PARALLEL_CENTROID_H