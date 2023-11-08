#ifndef K_MEANS_SEQUENTIAL_CENTROID_H
#define K_MEANS_SEQUENTIAL_CENTROID_H

#include <vector>


namespace Sequential {
  // Centroid in multidimensional space.
  struct Centroid {
    const int dimensions; // Number of dimensions.

    std::vector<double> coordinates; // Vector of coordinates.
    int clusterId; // Identifier of the cluster.


    /*
      * Centroid constructor.
      * 
      * @param dimensions: Number of dimensions.
      * @param coordinates: Vector of coordinates.
      * @param clusterId: Identifier of the cluster.
    */
    Centroid(const int dimensions, const std::vector<double>& coordinates, const int clusterId);

  };
}

#endif // K_MEANS_SEQUENTIAL_CENTROID_H