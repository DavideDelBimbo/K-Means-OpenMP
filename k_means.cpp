#include <vector>
#include <random>
#include <cmath>
#include <omp.h>
#include <iostream>

#include "k_means.h"
#include "params.h"


KMeans::KMeans(int N, int K) : N(N), K(K) {
    points.reserve(N); // Reserve space for N points.
    centroids.reserve(K); // Reserve space for K centroids.
}


void KMeans::initializePoints() {
    // Uniform distribution between 0 and MAX_RANGE.
    std::default_random_engine generator(SEED); // Random number engine (with seed for reproducibility).
    std::uniform_real_distribution<double> uniform_distribution(0, MAX_RANGE); // Uniform distribution.

    // Generate N random points from the uniform distribution.
    for(int i=0; i<N; i++) {
        double x = uniform_distribution(generator);
        double y = uniform_distribution(generator);

        // Add the point to the vector.
        points.push_back(Point(x, y));
    }
}

void KMeans::initializeClusters() {
    // Set seed for reproducibility.
    srand(SEED);

    // Generate K random clusters with centroids chosen from the points.
    for(int i=0; i<K; i++) {
        Point* point = &points[rand() % N]; // Random point from the points vector.

        // Add the centroid to the vector.
        centroids.push_back(Centroid(point->x, point->y));
    }
}


double KMeans::distance(const Point &p, const Centroid &c) {
    return sqrt((p.x - c.x)*(p.x - c.x) + (p.y - c.y)*(p.y - c.y));
}

bool KMeans::isConverged(const std::vector<int>& previousAssignments) {
    // Check if the points have changed cluster.
    for (int i = 0; i < N; i++) {
        // No convergence if a point has changed cluster.
        if (points[i].cluster != previousAssignments[i]) {
            return false;
        }
    }

    // Convergence if no point has changed cluster.
    return true;
}


void KMeans::kMeansIterationSequential() {
    // Variables for the mean of the points in each cluster.
    double sum_x[K] = {0}; // Sum of x coordinates of points in each cluster.
    double sum_y[K] = {0}; // Sum of y coordinates of points in each cluster.
    int sizes[K] = {0}; // Number of points in each cluster.

    // Assign each point to the closest centroid.
    for(int i=0; i<N; i++) {
        int clusterIdx = -1; // Index of the closest cluster (initialize to -1).
        double minDist = std::numeric_limits<int>::max(); // Distance to the closest cluster (initialized to infinity).

        for(int j=0; j<K; j++) {
            double dist = distance(points[i], centroids[j]);

            if(dist < minDist) {
                minDist = dist;
                clusterIdx = j;
            }
        }

        points[i].cluster = clusterIdx;

        // Calculate the mean of the points in each cluster.
        sum_x[clusterIdx] += points[i].x; // Sum the x coordinate of the point assigned to the cluster.
        sum_y[clusterIdx] += points[i].y; // Sum the y coordinate of the point assigned to the cluster.
        sizes[clusterIdx]++; // Increment the size of the cluster.
    }
        
    // Update the centroids.
    for(int i=0; i<K ; i++){
        // Update the centroid of the cluster.
        centroids[i].x = sum_x[i] / sizes[i];
        centroids[i].y = sum_y[i] / sizes[i];
    }
}
