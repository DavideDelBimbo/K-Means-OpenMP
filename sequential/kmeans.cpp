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


namespace Sequential {
    KMeans::KMeans(const int n, const int k, const int d) : N(n), K(k), dimensions(d), points (initializeRandomPoints()), centroids(initializeCentroids()) { }

    KMeans::KMeans(const std::string& filePath, const int k) : filePath(filePath), K(k), points(initializeInputPoints()), centroids(initializeCentroids()) { }


    void KMeans::run(const std::string &basePath, const bool log) {
        std::cout << "Running sequential k-means with " << N << " points and " << K << " clusters." << std::endl;

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
        paths = create_folders(basePath, "sequential", N, K, dimensions, canPlot);

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
                log_data(iterations, paths, "sequential", "points", getCoordinates(points), getIds(points));

                log_data(iterations, paths, "sequential", "centroids", getCoordinates(centroids), getIds(centroids));
                
                // Plot the points and the centroids.
                plot_data(iterations, paths, "sequential", initMode, N, K, dimensions);
            }

            iterations++;
        }

        if(canPlot) {
            // Convert to gif.
            convert_gif(iterations, executionTimes, paths, "sequential");
        }

        std::cout << "Converged after " << iterations << " iterations in " << executionTimes << " seconds." << std::endl;

        // Save the results.
        save_results(iterations, executionTimes, paths, "sequential", N, K, dimensions);
    }


    const std::vector<Point> KMeans::initializeRandomPoints() {
        // Uniform distribution between 0 and MAX_RANGE.
        std::default_random_engine generator(SEED); // Random number engine (with seed for reproducibility).
        std::uniform_real_distribution<double> uniformDistribution(0, MAX_RANGE); // Uniform distribution.

        // Initialize vector of points.
        std::vector<Point> points(N, Point(dimensions, std::vector<double>(dimensions, 0), 0, -1));

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

    const std::vector<Point> KMeans::initializeInputPoints() {
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

        // Initialize vector of points.
        std::vector<Point> points(N, Point(dimensions, std::vector<double>(dimensions, 0), 0, -1));

        int i = 0;
        while (getline(file, line)) {
            std::stringstream str(line);
            int dim = 0;
            while (getline(str, word, ',')) {
                // Set the coordinate to the point.
                points[i].coordinates[dim] = std::stod(word);

                // Increment the dimension.
                dim++;
            }

            // Set the identifier of the point.
            points[i].pointId = i;

            // Increment the point.
            i++;
        }

        return points;
    }

    const std::vector<Centroid> KMeans::initializeCentroids() {
        if (K > N) {
            throw std::runtime_error("ERROR: K cannot be greater than N!");
        }

        // Uniform distribution between 0 and N-1 for selecting unique indices.
        std::default_random_engine generator(SEED); // Random number engine (with seed for reproducibility).
        std::uniform_int_distribution<int> intDistribution(0, N - 1); // Uniform distribution.

        // Vector of random indices.
        std::set<int> randomIndices;

        // Generate K random indices.
        while (randomIndices.size() < K) {
            int randomIndex = intDistribution(generator);

            // Check if the random index is unique.
            if (randomIndices.find(randomIndex) == randomIndices.end()) {
                randomIndices.insert(randomIndex);
            }
        }

        // Initialize vector of centroids.
        std::vector<Centroid> centroids(K, Centroid(dimensions, std::vector<double>(dimensions, 0), 0));

        // Generate K random centroids from points.
        for(int j = 0; j < K; j++) {
            // Get the j-th random index.
            int randomIndex = *std::next(randomIndices.begin(), j);

            for(int dim = 0; dim < dimensions; dim++) {
                // Set the coordinates of the centroid.
                centroids[j].coordinates[dim] = points[randomIndex].coordinates[dim];
            }

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


    bool KMeans::KMeansIteration() {
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

            for(int dim = 0; dim < dimensions; dim++) {
                // Sum the coordinates of the point assigned to the cluster.
                clustersSum[minClusterId][dim] += points[i].coordinates[dim];
            }

            // Increment the size of the cluster.
            clustersSize[minClusterId]++;
        }
            
        // Update the centroids.
        for(int j = 0; j < K; j++){
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
        // Initialize the coordinates vector.
        std::vector<std::vector<double>> coordinates(data.size(), std::vector<double>(dimensions, 0));
        
        // Fill the coordinates vector.
        for(int i = 0; i < data.size(); i++) {
            for(int dim = 0; dim < dimensions; dim++) {
                coordinates[i][dim] = data[i].coordinates[dim];
            }
        }

        return coordinates;
    }

    template <typename T>
    const std::vector<int> KMeans::getIds(const std::vector<T>& data) {
        // Initialize the ids vector.
        std::vector<int> ids(data.size(), 0);
        
        // Fill the ids vector.
        for(int i = 0; i < data.size(); i++) {
            ids[i] = data[i].clusterId;
        }

        return ids;
    }
}