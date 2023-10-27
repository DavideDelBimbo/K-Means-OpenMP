#ifndef K_MEANS_PARALLEL_H
#define K_MEANS_PARALLEL_H

#include <vector>

#include "points.h"
#include "centroids.h"


namespace Parallel {
    class KMeans {
        public:
            /*
                * KMeans constructor.
                * 
                * @param N: Number of points.
                * @param K: Number of clusters.
                * @param dimensions: Number of dimensions.
                * @param threads: Number of threads.
            */
            KMeans(const int N, const int K, const int dimensions, const int threads);


            /*
                * Execution of the k-means algorithm.
            */
            void run();


            /*
                * Get the coordinates of the data.
                *
                * @param data: The data.
                * 
                * @returns (std::vector<std::vector<double>>) The coordinates of the data.
            */
            template <typename T>
            const std::vector<std::vector<double>> getCoordinates(const T& data);

            /*
                * Get the identifiers of the data.
                *
                * @param data: The data.
                * 
                * @returns (std::vector<std::vector<int>>) The identifiers of the data.
            */
            template <typename T>
            const std::vector<int> getIds(const T& data);

        private:
            int N; // Number of points.
            int K; // Number of clusters.
            int dimensions; // Number of dimensions.
            int threads; // Number of threads.

            Points points; // Vector of points.
            Centroids centroids; // Vector of centroids.


            /*
                * Initializes the points with random coordinates.
                *
                * @returns (Points) The points.
            */
            Points initializePoints();

            /*
                * Initializes the centroids with k random points.
                *
                * @returns (Centroids) The centroids.
            */
            Centroids initializeCentroids();


            /*
                * Calculates the Euclidean distance between a point and a centroid.
                * 
                * @param pointId: The identifier of the point.
                * @param centroidId: The identifier of the centroid.
                * 
                * @returns (double) The distance between the point and the centroid.
            */
            const double distance(const int pointId, const int centroidId);


            /*
                * Performs a single iteration of the k-means algorithm.
                * 
                * @returns (bool) True if the centroids have changed, false otherwise.
            */
            bool kMeansIteration();
    };
}

#endif // K_MEANS_PARALLEL_H