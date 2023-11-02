#ifndef K_MEANS_PARALLEL_POINT_H
#define K_MEANS_PARALLEL_POINT_H

#include <ostream>
#include <vector>


namespace Parallel {
  // Points in multidimensional space using SoA format.
  struct Points {
    int size; // Number of points.
    int dimensions; // Number of dimensions.
    
    std::vector<double> coordinates; // Vector for coordinates of all dimensions (x1, x2, x3, ..., y1, y2, y3, ..., z1, z2, z3, ...).
    std::vector<int> pointsIds; // Identifiers of the points.
    std::vector<int> clustersIds; // Identifiers of the clusters to which the points belong.


    /*
      * Points default constructor.
    */
    Points() = default;

    /*
      * Points constructor.
      * 
      * @param size: Number of points.
      * @param dimensions: Number of dimensions.
      * @param coordinates: Vectors of coordinates of the points.
      * @param pointdIds: Identifiers of the points.
    */
    Points(const int size, const int dimensions, const std::vector<double>& coordinates, const std::vector<int>& pointsIds);
  };
}

#endif // K_MEANS_PARALLEL_POINT_H