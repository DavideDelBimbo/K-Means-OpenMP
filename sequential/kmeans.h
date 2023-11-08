#ifndef K_MEANS_SEQUENTIAL_H
#define K_MEANS_SEQUENTIAL_H

#include <vector>

#include "point.h"
#include "centroid.h"


namespace Sequential {
    class KMeans {
        public:
            /*
                * KMeans constructor.
                * 
                * @param N: Number of points.
                * @param K: Number of clusters.
                * @param dimensions: Number of dimensions.
            */
            KMeans(const int N, const int K, const int dimensions);

            /*
                * KMeans constructor.
                * 
                * @param filePath: Path of the file with the points.
                * @param K: Number of clusters.
            */
            KMeans(const std::string& filePath, const int K);


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
            const std::vector<std::vector<double>> getCoordinates(const std::vector<T>& data);

            /*
                * Get the identifiers of the data.
                *
                * @param data: The data.
                * 
                * @returns (std::vector<std::vector<int>>) The identifiers of the data.
            */
            template <typename T>
            const std::vector<int> getIds(const std::vector<T>& data);

        private:
            const std::string filePath = ""; // Path of the file with the points.
            int N; // Number of points.
            const int K; // Number of clusters.
            int dimensions; // Number of dimensions.

            std::vector<Point> points; // Vector of points.
            std::vector<Centroid> centroids; // Vector of centroids.


            /*
                * Initializes the points with random coordinates.
                *
                * @returns (std::vector<Point>) Vector of points.
            */
            const std::vector<Point> initializeRandomPoints();

            /*
                * Initializes the points with form input file.
                *
                * @returns (std::vector<Point>) Vector of points.
            */
            const std::vector<Point> initializeInputPoints();

            /*
                * Initializes the centroids with k random points.
                *
                * @returns (std::vector<Centroid>) Vector of centroids.
            */
            const std::vector<Centroid> initializeCentroids();


            /*
                * Calculates the Euclidean distance between a point and a centroid.
                * 
                * @param p: The point.
                * @param c: The centroid.
                * 
                * @returns (double) The distance between the point and the centroid.
            */
            const double distance(const Point& p, const Centroid &c);


            /*
                * Performs a single iteration of the k-means algorithm.
                * 
                * @returns (bool) True if the centroids have changed, false otherwise.
            */
            bool KMeansIteration();
    };
}

#endif // K_MEANS_SEQUENTIAL_H