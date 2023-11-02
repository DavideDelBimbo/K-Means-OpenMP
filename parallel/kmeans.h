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
                * KMeans constructor.
                * 
                * @param filePath: Path of the file with the points.
                * @param K: Number of clusters.
                * @param threads: Number of threads.
            */
            KMeans(const std::string& filePath, const int K, const int threads);


            /*
                * Execution of the k-means algorithm.
                *
                * @param base_path: The base path for the results.
                * @param log: True if the results should be logged, false otherwise.
            */
            void run(const std::string &base_path = "results\\", const bool log = false);


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
            const int K; // Number of clusters.
            int dimensions; // Number of dimensions.
            const int threads; // Number of threads.
            const std::string filePath = ""; // Path of the file with the points.

            Points points; // Vector of points.
            Centroids centroids; // Vector of centroids.


            /*
                * Initializes the points with random coordinates.
                *
                * @returns (Points) The points.
            */
            const Points initializeRandomPoints();

            /*
                * Initializes the points with form input file.
                *
                * @returns (std::vector<Point>) Vector of points.
            */
            const Points initializeInputPoints();

            /*
                * Initializes the centroids with k random points.
                *
                * @returns (Centroids) The centroids.
            */
            const Centroids initializeCentroids();


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