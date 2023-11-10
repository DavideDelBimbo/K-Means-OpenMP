#ifndef K_MEANS_PARALLEL_POINT_H
#define K_MEANS_PARALLEL_POINT_H


namespace Parallel {
  // Points in multidimensional space using SoA architecture.
  struct Points {
    const int size; // Number of points.
    const int dimensions; // Number of dimensions.
    
    double* coordinates; // Array of coordinates of all dimensions (x1, x2, x3, ..., y1, y2, y3, ..., z1, z2, z3, ...).
    int* pointsIds; // Array of points identifiers.
    int* clustersIds; // Array of clusters identifiers to which the points belong.


    /*
      * Points constructor.
      * 
      * @param size: Number of points.
      * @param dimensions: Number of dimensions.
      * @param coordinates: Array of points coordinates.
      * @param pointdIds: Array of points identifiers.
      * @param clustersIds: Array of clusters identifiers to which the points belong.
    */
    Points(const int size, const int dimensions, double* coordinates, int* pointsIds, int* clustersIds);

    /*
      * Points destructor.
    */
    ~Points();
  };
}

#endif // K_MEANS_PARALLEL_POINT_H