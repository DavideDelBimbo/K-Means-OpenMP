#include <iostream>
#include <chrono>

#include "params.h"
#include "k_means.h"


int main() {
    int N = NUM_POINTS;
    int K = NUM_CLUSTERS;
    int maxIterations = MAX_ITERATIONS;

    // Variables for convergence.
    int iteration = 0; // Number of iterations executed.
    bool converged = false;  // Convergence flag.
    std::vector<int> previousAssignments(N, -1); // Vector of previous assignments of the points to clusters.
    std::vector<double> times; // Vector of times.

    // Initialize the K-Means algorithm.
    KMeans kmeans(N, K);
    kmeans.initializePoints();
    kmeans.initializeClusters();

    // Run the K-Means algorithm.
    for(iteration=0; iteration<maxIterations && !converged; iteration++) {
        // Start the timer.
        const auto start = std::chrono::high_resolution_clock::now();

        // Run one iteration of the K-Means algorithm.
        kmeans.kMeansIterationSequential();

        // Get the points and clusters.
        std::vector<Point>& points = kmeans.getPoints();
        std::vector<Centroid>& clusters = kmeans.getClusters();

        // Check if the algorithm has converged.
        converged = kmeans.isConverged(previousAssignments);

        // Update the previous assignments.
        for (int i = 0; i<N; i++) {
            previousAssignments[i] = points[i].cluster;
        }

        // Stop the timer.
        const auto end = std::chrono::high_resolution_clock::now();
        const double elapsed_seconds = std::chrono::duration<double>(end - start).count();

        // Update the times vector.
        times.push_back(elapsed_seconds);

        std::cout << "Iteration " << iteration << " took " << elapsed_seconds << " seconds" << std::endl;   
    }

    std::cout << "Converged in " << iteration << " iterations" << std::endl;

    return 0;
}