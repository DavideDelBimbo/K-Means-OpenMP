#include <iostream>
#include <chrono>

#include "params.h"
#include "k_means.h"
#include "utils.h"


int main() {
    // Parameters.
    int N = NUM_POINTS;
    int K = NUM_CLUSTERS;
    int maxIterations = MAX_ITERATIONS;

    // Variables for convergence.
    int iteration = 0; // Number of iterations executed.
    bool converged = false;  // Convergence flag.

    std::cout << "Running K-Means with " << N << " points and " << K << " clusters in " << (PARALLEL ? "parallel" : "sequential") << " mode." << std::endl;

    // Start the total timer.
    const auto start_total = std::chrono::high_resolution_clock::now();

    // Initialize the K-Means algorithm.
    KMeans kmeans(N, K);
    kmeans.initializePoints();
    kmeans.initializeClusters();

    // Run the K-Means algorithm.
    for(iteration=0; iteration<maxIterations && !converged; iteration++) {
        // Start the execution timer.
        const auto start = std::chrono::high_resolution_clock::now();

        // Run one iteration of the K-Means algorithm.
        (!PARALLEL) ? converged = kmeans.kMeansIterationSequential() : converged = kmeans.kMeansIterationParallel();

        // Stop the execution timer.
        const auto end = std::chrono::high_resolution_clock::now();
        const double elapsed_seconds = std::chrono::duration<double>(end - start).count();

        std::cout << "\t-Iteration " << iteration << " took " << elapsed_seconds << " seconds." << std::endl;

        // Log the results.
        if(LOG) {
            // Get the points and clusters.
            const std::vector<Point>& points = kmeans.getPoints();
            const std::vector<Centroid>& clusters = kmeans.getClusters();

            // Log the results.
            log(iteration, points, clusters, elapsed_seconds, PARALLEL);
        }

        
    }
    std::cout << "Converged after " << iteration << " iterations." << std::endl;

    // Stop the total timer.
    const auto end_total = std::chrono::high_resolution_clock::now();
    const double elapsed_seconds_total = std::chrono::duration<double>(end_total - start_total).count();

    std::cout << "Total execution took " << elapsed_seconds_total << " seconds." << std::endl;

    return 0;
}