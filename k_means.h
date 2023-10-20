#ifndef K_MEANS_H
#define K_MEANS_H

#include <vector>

#include "point.h"
#include "centroid.h"


class KMeans {
public:
    /*
    KMeans constructor.

    Parameters:
        - N (int): number of points.
        - K (int): number of clusters.
    */
    KMeans(int N, int K);


    /*
    Initialize a vector of points.
    */
    void initializePoints();

    /*
    Initialize a vector of clusters.
    */
    void initializeClusters();


    /*
    Method to calculate the Euclidean distance between a point and a centroid.
    
    Parameters:
        - p (Point): point to which the distance is calculated.
        - c (Centroid): centroid to which the distance is calculated.

    Returns:
        - (double): Euclidean distance between the point and the centroid.
    */
    double distance(const Point& p, const Centroid& c);

    /*
    Method to check if the algorithm has converged.

    Parameters:
        - previousAssignments (std::vector<int>): vector of previous assignments of the points to clusters.
    
    Returns:
        - (bool): true if the algorithm has converged, false otherwise.
    */
    bool isConverged(const std::vector<int>& previousAssignments);

    
    /*
    Method to run one iteration of the K-Means algorithm in sequential. 
    */
    void kMeansIterationSequential();
    

    /*
    Getter methods for the points.

    Returns:
        - (std::vector<Point> &): reference to the vector of points.
    */
    std::vector<Point>& getPoints() { return points; }

    /*
    Getter methods for the clusters.

    Returns:
        - (std::vector<Centroid> &): reference to the vector of clusters.
    */
    std::vector<Centroid>& getClusters() { return centroids; }
private:
    int N; // Number of points.
    int K; // Number of clusters.

    std::vector<Point> points; // Vector of points.
    std::vector<Centroid> centroids; // Vector of centroids.
};

#endif // K_MEANS_H