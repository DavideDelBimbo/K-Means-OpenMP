#include <iostream>
#include <random>
#include <cmath>
#include <vector>
#include <float.h>
#include <omp.h>

#include "kmeans.h"
#include "../utils.h"
#include "../params.h"


namespace Parallel {
    KMeans::KMeans(const int N, const int K, const int d, const int t) : N(N), K(K), dimensions(d), threads(t), points(initializePoints()), centroids(initializeCentroids()) { };


    void KMeans::run() {
        // Set the number of threads.
        omp_set_num_threads(threads);

        std::cout << "Running parallel k-means with " << N << " points and " << K << " clusters using #" << omp_get_max_threads() << " threads." << std::endl;

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
            paths = create_folders("results\\parallel\\threads_" + std::to_string(omp_get_max_threads()) + "\\dimensions_" + std::to_string(dimensions) + "\\clusters_" + std::to_string(K) + "\\points_" + std::to_string(N) + "\\");
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
                log_data(iterations, paths, "parallel", "points", getCoordinates(points), getIds(points));

                log_data(iterations, paths, "parallel", "centroids", getCoordinates(centroids), getIds(centroids));
                
                // Plot the points and the centroids.
                plot_data(iterations, paths, "parallel", N, K, dimensions);
            }

            iterations++;
        }

        if(canPlot) {
            // Convert to gif.
            convert_gif(iterations, executionTimes, paths, "parallel");
        }

        std::cout << "Converged after " << iterations << " iterations in " << executionTimes << " seconds." << std::endl;

        // Save the results.
        save_results(iterations, executionTimes, ".\\results\\parallel\\", "parallel", N, K, dimensions);
    }


    Points KMeans::initializePoints() {
        // Uniform distribution between 0 and MAX_RANGE.
        std::default_random_engine generator(SEED); // Random number engine (with seed for reproducibility).
        std::uniform_real_distribution<double> uniformDistribution(0, MAX_RANGE); // Uniform distribution.

        // Initialize Point structure.
        Points points(N, dimensions, std::vector<double>(N * dimensions, 0), std::vector<int>(N, 0));

        // Generate N random points from the uniform distribution.
        for(int i = 0; i < N; i++) {
            for(int dim = 0; dim < dimensions; dim++) {
                // Generate a random coordinate.
                points.coordinates[i + N * dim] = uniformDistribution(generator);
            }

            // Set the identifier of the point.
            points.pointsIds[i] = i;
        }

        return points;
    }

    Centroids KMeans::initializeCentroids() {
        // Uniform distribution between 0 and N-1 for selecting unique indices.
        std::default_random_engine generator(SEED); // Random number engine (with seed for reproducibility).
        std::uniform_int_distribution<int> intDistribution(0, N - 1); // Uniform distribution.

        // Initialize Centroids structure.
        Centroids centroids(K, dimensions, std::vector<double>(K * dimensions, 0), std::vector<int>(K, 0));

        // Generate K random centroids from points.
        for(int j = 0; j < K; j++) {
            int randomIndex = intDistribution(generator);

            for(int dim = 0; dim < dimensions; dim++) {
                // Set the coordinates of the centroid.
                centroids.coordinates[j + K * dim] = points.coordinates[randomIndex + N * dim];
            }

            // Set the identifier of the centroid.
            centroids.clustersIds[j] = j;
        }

        return centroids;
    }


    const double KMeans::distance(const int pointId, const int centroidId) {
        double sum = 0;
        #pragma omp simd
        for (int dim = 0; dim < dimensions; dim++) {
            sum += (centroids.coordinates[centroidId + K * dim] - points.coordinates[pointId + N * dim]) * (centroids.coordinates[centroidId + K * dim] - points.coordinates[pointId + N * dim]);
        }
        
        return sqrt(sum);
    }


    bool KMeans::kMeansIteration() {
        // Variables for the mean of the points in each cluster.
        std::vector<double> clustersSum(K * dimensions, 0); // Sum of coordinates of points in each cluster.
        std::vector<int> clustersSize(K, 0); // Number of points in each cluster.

        // Convergence flag. Assume convergence at the beginning.
        bool converged = true;

        // Assign each point to the closest centroid.
        #pragma omp parallel default(none) shared(points, centroids, clustersSum, clustersSize, converged)
        {
            #pragma omp for schedule(static)
            for(int i = 0; i < N; i++) {
                double minDist = DBL_MAX; // Distance to the closest cluster (initialized to infinity).
                int minClusterId = -1; // Id of the closest cluster (initialize to -1).

                for(int j = 0; j < K; j++) {
                    double dist = distance(i, j);

                    if(dist < minDist) {
                        minDist = dist;
                        minClusterId = j;
                    }
                }

                // Assign the point to the closest cluster.
                points.clustersIds[i] = minClusterId;

                // Calculate the mean of the points in each cluster.
                for(int dim = 0; dim < dimensions; dim++) {
                    #pragma omp atomic
                    // Sum the coordinates of the point assigned to the cluster.
                    clustersSum[minClusterId + K * dim] += points.coordinates[i + N * dim];
                }

                #pragma omp atomic
                // Increment the size of the cluster.
                clustersSize[minClusterId]++;
            }
            
            #pragma omp for schedule(static) nowait
            // Update the centroids.
            for(int j = 0; j < K ; j++){
                // Temporary vector for the previous coordinates of the centroid.
                std::vector<double> tmpCoordinates(dimensions, 0);

                // Update the centroid of the cluster.
                for(int dim = 0; dim < dimensions; dim++) {
                    // Save the previous centroid coordinates.
                    tmpCoordinates[dim] = centroids.coordinates[j + K * dim];

                    // Calculate the new centroid coordinates.
                    centroids.coordinates[j + K * dim] = clustersSum[j + K * dim] / clustersSize[j];
                
                    // Check for convergence.
                    if (fabs(tmpCoordinates[dim] - centroids.coordinates[j + K * dim]) > EPSILON) {
                        converged = false;
                    }
                }
            }
        }

        return converged;
    }


    template <typename T>
    const std::vector<std::vector<double>> KMeans::getCoordinates(const T& data) {
        std::vector<std::vector<double>> coordinates(data.size, std::vector<double>(data.dimensions, 0));
        
        for(int i = 0; i < data.size; i++) {
            for(int dim = 0; dim < data.dimensions; dim++) {
                coordinates[i][dim] = data.coordinates[i + data.size * dim];
            }
        }

        return coordinates;
    }

    template <typename T>
    const std::vector<int>  KMeans::getIds(const T& data) {
        std::vector<int> ids(data.size, 0);
        
        for(int i = 0; i < data.size; i++) {
            ids[i] = data.clustersIds[i];
        }

        return ids;
    }

}