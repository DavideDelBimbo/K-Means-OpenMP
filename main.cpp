#include <iostream>

#include "params.h"
#include "sequential/kmeans.h"
#include "parallel/kmeans.h"


int main() {
    /*int N = NUM_POINTS;
    int K = NUM_CLUSTERS;
    int d = DIMENSIONS;


    if (!PARALLEL) {
        Sequential::KMeans(N, K, d).run();
    } else {
        Parallel::KMeans(N, K, d).run();
    }*/
    std::vector<int> n = {10'000, 50'000, 100'000, 500'000, 1'000'000, 5'000'000, 10'000'000};
    std::vector<int> k = {5, 10, 20, 30, 100};
    std::vector<int> d = {2, 3, 5, 10, 30};
    std::vector<int> t = {1, 2, 4, 6, 8, 12};
    
    for (auto D : d) {
        for (auto K : k) {
            for (auto N : n) {
                Sequential::KMeans(N, K, D).run();
                for (auto T : t) {
                    Parallel::KMeans(N, K, D, T).run();
                }
            }
        }
    }

}