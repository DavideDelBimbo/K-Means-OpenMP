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
            int N; // Number of points.
            int K; // Number of clusters.
            int dimensions; // Number of dimensions.

            std::vector<Point> points; // Vector of points.
            std::vector<Centroid> centroids; // Vector of centroids.


            /*
                * Initializes the points with random coordinates.
                *
                * @returns (std::vector<Point>) Vector of points.
            */
            std::vector<Point> initializePoints();

            /*
                * Initializes the centroids with k random points.
                *
                * @returns (std::vector<Centroid>) Vector of centroids.
            */
            std::vector<Centroid> initializeCentroids();


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
            bool kMeansIteration();
    };
}

#endif // K_MEANS_SEQUENTIAL_H