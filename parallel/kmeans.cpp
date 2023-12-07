#include <iostream>
#include <random>
#include <cmath>
#include <vector>
#include <set>
#include <float.h>
#include <omp.h>

#include "kmeans.h"
#include "../utils.h"
#include "../params.h"


namespace Parallel {
    KMeans::KMeans(const int n, const int k, const int d, const int t) : N(n), K(k), dimensions(d), threads(t), points(initializeRandomPoints()), centroids(initializeCentroids()) { }

    KMeans::KMeans(const std::string& filePath, const int k, const int t) : filePath(filePath), K(k), threads(t), points(initializeInputPoints()), centroids(initializeCentroids()) { }


    void KMeans::run(const std::string &basePath, const bool log) {
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
        std::string initMode = filePath.empty() ? "random" : "input";
        bool canPlot = log && (dimensions == 2 || dimensions == 3);
        if (!canPlot) {
            std::cout << "LOG is disabled (" << (log ? "LOG=true" : "LOG=false") << ") or cannot plot points with specified dimensions (DIMENSIONS=" << dimensions << ")." << std::endl;
        }

        // Create the folders for the results.
        paths = create_folders(basePath, "parallel", N, K, dimensions, canPlot);

        while (iterations < MAX_ITERATIONS && !converged) {
            // Start the timer.
            double startTime = omp_get_wtime();

            // Execute the iteration and check if the centroids have changed.
            converged = KMeansIteration();

            // Stop the timer.
            double endTime = omp_get_wtime();
            executionTimes += endTime - startTime;

            if (canPlot) {
                // Log the iteration.
                log_data(iterations, paths, "parallel", "points", getCoordinates(points), getClustersIds(points));

                log_data(iterations, paths, "parallel", "centroids", getCoordinates(centroids), getClustersIds(centroids));
                
                // Plot the points and the centroids.
                plot_data(iterations, paths, "parallel", initMode, N, K, dimensions);
            }

            iterations++;
        }

        if(canPlot) {
            // Convert to gif.
            convert_gif(iterations, executionTimes, paths, "parallel");
        }

        std::cout << "Converged after " << iterations << " iterations in " << executionTimes << " seconds." << std::endl;

        // Save the results.
        save_results(iterations, executionTimes, paths, "parallel", N, K, dimensions);
    }


    const Points KMeans::initializeRandomPoints() {
        // Uniform distribution between 0 and MAX_RANGE.
        std::default_random_engine generator(SEED); // Random number engine (with seed for reproducibility).
        std::uniform_real_distribution<double> uniformDistribution(0, MAX_RANGE); // Uniform distribution.

        // Initialize Point structure.
        Points points(N, dimensions, new double[N * dimensions], new int[N], new int[N]);

        // Generate N random points from the uniform distribution.
        for(int i = 0; i < N; i++) {
            for(int dim = 0; dim < dimensions; dim++) {
                // Generate a random coordinate.
                points.coordinates[i + N * dim] = uniformDistribution(generator);
            }

            // Set the identifier of the point.
            points.pointsIds[i] = i;

            // Set the identifier of the cluster.
            points.clustersIds[i] = -1;
        }

        return points;
    }

    const Points KMeans::initializeInputPoints() {
        // File stream.
        std::ifstream file;
        std::string line, word;

        // Open the file.
        file.open(filePath, std::ios::in);

        if (!file.is_open()) {
            throw std::runtime_error("ERROR: couldn't open file");
        }

        // Count the number of lines and columns in the file.
        int numColumns = 0;
        std::getline(file, line);
        std::stringstream ss(line);
        while (std::getline(ss, line, ',')) numColumns++;
        
        // Count the number of lines in the file.
        int numLines = 1;
        while (std::getline(file, line)) numLines++;

        // Set N and dimensions based on the file content.
        N = numLines; // Number of points.
        dimensions = numColumns; // Number of dimensions.


        // Reopen the file for reading from the beginning.
        file.clear();
        file.seekg(0, std::ios::beg);

        // Initialize points structure.
        Points points(N, dimensions, new double[N * dimensions], new int[N], new int[N]);

        int i = 0;
        while (getline(file, line)) {
            std::stringstream str(line);
            int dim = 0;
            while (getline(str, word, ',')) {
                // Set the coordinate to the point.
                points.coordinates[i + N * dim] = std::stod(word);

                // Increment the dimension.
                dim++;
            }

            // Set the identifier of the point.
            points.pointsIds[i] = i;

            // Set the identifier of the cluster.
            points.clustersIds[i] = -1;

            // Increment the point.
            i++;
        }

        return points;
    }

    const Centroids KMeans::initializeCentroids() {
        if (K > N) {
            throw std::runtime_error("ERROR: K cannot be greater than N!");
        }

        // Uniform distribution between 0 and N-1 for selecting unique indices.
        std::default_random_engine generator(SEED); // Random number engine (with seed for reproducibility).
        std::uniform_int_distribution<int> intDistribution(0, N - 1); // Uniform distribution.

        // Set of random indices.
        std::set<int> randomIndices;

        // Generate K random indices.
        while (randomIndices.size() < K) {
            int randomIndex = intDistribution(generator);

            // Check if the random index is unique.
            if (randomIndices.find(randomIndex) == randomIndices.end()) {
                randomIndices.insert(randomIndex);
            }
        }


        // Initialize Centroids structure.
        Centroids centroids(K, dimensions, new double[K * dimensions], new int[K]);

        // Generate K random centroids from points.
        for(int j = 0; j < K; j++) {
            // Get the j-th random index.
            int randomIndex = *std::next(randomIndices.begin(), j);

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
        #pragma omp simd reduction(+:sum)
        for (int dim = 0; dim < dimensions; dim++) {
            sum += (centroids.coordinates[centroidId + K * dim] - points.coordinates[pointId + N * dim]) * (centroids.coordinates[centroidId + K * dim] - points.coordinates[pointId + N * dim]);
        }
        
        return sqrt(sum);
    }


    bool KMeans::KMeansIteration() {
        // Variables for the mean of the points in each cluster.
        double clustersSum[K * dimensions] = {0}; // Sum of coordinates of points in each cluster.
        int clustersSize[K] = {0}; // Number of points in each cluster.

        // Convergence flag. Assume convergence at the beginning.
        bool converged = true;

        // Assign each point to the closest centroid.
        #pragma omp parallel for schedule(static) default(none) shared(points, centroids, clustersSum, clustersSize) 
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

            // Update the identifier of the cluster.
            points.clustersIds[i] = minClusterId;

            for(int dim = 0; dim < dimensions; dim++) {
                #pragma omp atomic
                // Sum the coordinates of the point assigned to the cluster.
                clustersSum[minClusterId + K * dim] += points.coordinates[i + N * dim];
            }
                    
            #pragma omp atomic
            // Increment the size of the cluster.
            clustersSize[minClusterId]++;
        }

        // Update the centroids.
        for(int j = 0; j < K; j++){
            // Temporary array for the previous coordinates of the centroid.
            double tmpCoordinates[dimensions] = {0};

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
        

        return converged;
    }


    template <typename T>
    const std::vector<std::vector<double>> KMeans::getCoordinates(const T& data) {
        // Initialize the coordinates vector.
        std::vector<std::vector<double>> coordinates(data.size, std::vector<double>(data.dimensions, 0));
        
        // Fill the coordinates vector.
        for(int i = 0; i < data.size; i++) {
            for(int dim = 0; dim < data.dimensions; dim++) {
                coordinates[i][dim] = data.coordinates[i + data.size * dim];
            }
        }

        return coordinates;
    }

    template <typename T>
    const std::vector<int>  KMeans::getClustersIds(const T& data) {
        // Initialize the clusters ids vector.
        std::vector<int> ids(data.size, 0);
        
        // Fill the clusters ids vector.
        for(int i = 0; i < data.size; i++) {
            ids[i] = data.clustersIds[i];
        }

        return ids;
    }
}