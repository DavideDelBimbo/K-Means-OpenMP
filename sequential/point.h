#ifndef K_MEANS_SEQUENTIAL_POINT_H
#define K_MEANS_SEQUENTIAL_POINT_H

#include <ostream>
#include <vector>


namespace Sequential {
  // Point in multidimensional space.
  struct Point {
    int dimensions; // Number of dimensions.

    std::vector<double> coordinates; // Vector of coordinates.
    int pointId; // Identifier of the point.
    int clusterId; // Identifier of the cluster to which the point belongs.


    /*
      * Point default constructor.
    */
    Point() = default;

    /*
      * Point constructor.
      * 
      * @param dimensions: Number of dimensions.
      * @param coordinates: Vector of coordinates.
      * @param pointId: Identifier of the point.
    */
    Point(const int dimensions, const std::vector<double>& coordinates, const int pointId);
  };
}

#endif // K_MEANS_SEQUENTIAL_POINT_H