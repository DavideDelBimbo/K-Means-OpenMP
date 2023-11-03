#ifndef K_MEANS_PARALLEL_CENTROID_H
#define K_MEANS_PARALLEL_CENTROID_H


namespace Parallel {
  // Centroids in multidimensional space using SoA format.
  struct Centroids {
    const int size; // Number of clusters.
    const int dimensions; // Number of dimensions.

    double* coordinates; // Array for coordinates of all dimensions (x1, x2, x3, ..., y1, y2, y3, ..., z1, z2, z3, ...).
    int* clustersIds; // Array for clusters identifiers.


    /*
      * Centroids constructor.
      * 
      * @param size: Number of clusters.
      * @param dimensions: Number of dimensions.
      * @param coordinates: Array of centroids coordinates.
      * @param centroidsIds: Array centroids identifiers.
    */
    Centroids(const int size, const int dimensions, double* coordinates, int* clustersIds);

    /*
      * Centroids destructor.
    */
    ~Centroids();
  };
}

#endif // K_MEANS_PARALLEL_CENTROID_H