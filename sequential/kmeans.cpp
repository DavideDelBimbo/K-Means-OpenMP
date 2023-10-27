#include <iostream>
#include <random>
#include <cmath>
#include <vector>
#include <float.h>
#include <omp.h>

#include "kmeans.h"
#include "../utils.h"
#include "../params.h"


namespace Sequential {
    KMeans::KMeans(const int N, const int K, int d) : N(N), K(K), dimensions(d), points(initializePoints()), centroids(initializeCentroids()) { };


    void KMeans::run() {
        std::cout << "Running sequential k-means with " << N << " points and " << K << " clusters." << std::endl;

        // Variables for convergence.
        int iterations = 0;
        bool converged = false;

        // Variable for execution times.
        double executionTimes = 0;
        FolderPaths paths;

        // Variable for logging.
        bool canPlot = LOG && (dimensions == 2 || dimensions == 3);
        if (!canPlot) {
            std::cout << "LOG is disabled (" << (LOG ? "LOG=true" : "LOG=false") << ") or cannot plot points with specified dimensions (DIMENSIONS=" << dimensions << ")." << std::endl;
        } else {
            // Create the folders for the results.
            paths = create_folders("results\\sequential\\dimensions_" + std::to_string(dimensions) + "\\clusters_" + std::to_string(K) + "\\points_" + std::to_string(N) + "\\");
        }

        while (iterations < MAX_ITERATIONS && !converged) {
            // Start the timer.
            double startTime = omp_get_wtime();

            // Execute the iteration and check if the centroids have changed.
            converged = kMeansIteration();

            // Stop the timer.
            double endTime = omp_get_wtime();
            executionTimes += endTime - startTime;

            if (canPlot) {
                // Log the iteration.
                log_data(iterations, paths, "sequential", "points", getCoordinates(points), getIds(points));

                log_data(iterations, paths, "sequential", "centroids", getCoordinates(centroids), getIds(centroids));
                
                // Plot the points and the centroids.
                plot_data(iterations, paths, "sequential", N, K, dimensions);
            }

            iterations++;
        }

        if(canPlot) {
            // Convert to gif.
            convert_gif(iterations, executionTimes, paths, "sequential");
        }

        std::cout << "Converged after " << iterations << " iterations in " << executionTimes << " seconds." << std::endl;

        // Save the results.
        save_results(iterations, executionTimes, ".\\results\\sequential\\", "sequential", N, K, dimensions);
    }


    std::vector<Point> KMeans::initializePoints() {
        // Uniform distribution between 0 and MAX_RANGE.
        std::default_random_engine generator(SEED); // Random number engine (with seed for reproducibility).
        std::uniform_real_distribution<double> uniformDistribution(0, MAX_RANGE); // Uniform distribution.

        // Initialize vector of points.
        std::vector<Point> points(N, Point(dimensions, std::vector<double>(dimensions, 0), 0));

        // Generate N random points from the uniform distribution.
        for(int i = 0; i < N; i++) {            
            for(int dim = 0; dim < dimensions; dim++) {
                // Generate a random coordinate.
                points[i].coordinates[dim] = uniformDistribution(generator);
            }

            // Set the identifier of the point.
            points[i].pointId = i;
        }

        return points;
    }

    std::vector<Centroid> KMeans::initializeCentroids() {
        // Uniform distribution between 0 and N-1 for selecting unique indices.
        std::default_random_engine generator(SEED); // Random number engine (with seed for reproducibility).
        std::uniform_int_distribution<int> intDistribution(0, N - 1); // Uniform distribution.

        // Initialize vector of centroids.
        std::vector<Centroid> centroids(K, Centroid(dimensions, std::vector<double>(dimensions, 0), 0));

        // Generate K random centroids from points.
        for(int j = 0; j < K; j++) {
            int randomIndex = intDistribution(generator);

            // Set the coordinates of the centroid.
            centroids[j].coordinates = points[randomIndex].coordinates;
            
            // Set the identifier of the centroid.
            centroids[j].clusterId = j;
        }

        return centroids;
    }


    const double KMeans::distance(const Point &p, const Centroid &c) {
        double sum = 0;
        for (int dim = 0; dim < dimensions; dim++) {
            sum += (c.coordinates[dim] - p.coordinates[dim]) * (c.coordinates[dim] - p.coordinates[dim]);
        }

        return sqrt(sum);
    }


    bool KMeans::kMeansIteration() {
        // Variables for the mean of the points in each cluster.
        std::vector<std::vector<double>> clustersSum(K, std::vector<double>(dimensions, 0)); // Sum of coordinates of points in each cluster.
        std::vector<int> clustersSize(K, 0); // Number of points in each cluster.

        // Convergence flag. Assume convergence at the beginning.
        bool converged = true;

        // Assign each point to the closest centroid.
        for(int i = 0; i < N; i++) {
            double minDist = DBL_MAX; // Distance to the closest cluster (initialized to infinity).
            int minClusterId = -1; // Id of the closest cluster (initialize to -1).

            for(int j = 0; j < K; j++) {
                double dist = distance(points[i], centroids[j]);

                if(dist < minDist) {
                    minDist = dist;
                    minClusterId = j;
                }
            }

            // Assign the point to the closest cluster.
            points[i].clusterId = minClusterId;

            // Calculate the mean of the points in each cluster.
            for(int dim = 0; dim < dimensions; dim++) {
                // Sum the coordinates of the point assigned to the cluster.
                clustersSum[minClusterId][dim] += points[i].coordinates[dim];
            }

            // Increment the size of the cluster.
            clustersSize[minClusterId]++;
        }
            
        // Update the centroids.
        for(int j = 0; j < K ; j++){
            // Temporary variable for the previous centroid coordinate.
            double tmpCoordinate = 0;

            // Update the centroid of the cluster.
            for(int dim = 0; dim < dimensions; dim++) {
                // Save the previous centroid coordinate.
                tmpCoordinate = centroids[j].coordinates[dim];

                centroids[j].coordinates[dim] = clustersSum[j][dim] / clustersSize[j];

                // Check for convergence (i.e. if the centroid change position in a dimension).
                if (fabs(tmpCoordinate - centroids[j].coordinates[dim]) > EPSILON) {
                    converged = false;
                }
            }
        }

        return converged;
    }


    template <typename T>
    const std::vector<std::vector<double>> KMeans::getCoordinates(const std::vector<T>& data) {
        std::vector<std::vector<double>> coordinates(data.size(), std::vector<double>(dimensions, 0));
        
        for(int i = 0; i < data.size(); i++) {
            coordinates[i] = data[i].coordinates;
        }

        return coordinates;
    }

    template <typename T>
    const std::vector<int> KMeans::getIds(const std::vector<T>& data) {
        std::vector<int> ids(data.size(), 0);
        
        for(int i = 0; i < data.size(); i++) {
            ids[i] = data[i].clusterId;
        }

        return ids;
    }
}