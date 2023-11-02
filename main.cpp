#include <iostream>
#include <cstring>

#include "params.h"
#include "sequential/kmeans.h"
#include "parallel/kmeans.h"


std::string INIT_MODE = "";
std::string FILE_PATH = "";
static int NUM_POINTS = 0;
static int NUM_CLUSTERS = 0;
static int DIMENSIONS = 0;
static std::string EXECUTION_TYPE = "";
static int NUM_THREADS = 0;
static std::string BASE_PATH = ".\\results\\";
static bool LOG = false;

void printHelp() {
    std::cout << "K-Means-OpenMP Help:" << std::endl;
    std::cout << "Usage: ./kmean [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  --help, -h: Display this help message." << std::endl;
    std::cout << "  --input_mode, -I: Input mode ('random' or 'input')." << std::endl;
    std::cout << "  --file_path, -F: Path to the input file (required only with '--input_mode=input')." << std::endl;
    std::cout << "  --num_points, -N: Number of points to generate (required only with '--input_mode=random')." << std::endl;
    std::cout << "  --num_clusters, -K: Number of clusters to generate." << std::endl;
    std::cout << "  --dimensions, -D: Number of dimensions for each data point." << std::endl;
    std::cout << "  --execution_type, -E: Execution type ('parallel' or 'sequential')." << std::endl;
    std::cout << "  --num_threads, -T: Number of threads to use for parallel execution." << std::endl;
    std::cout << "  --base_path, -B: Base path for the results (default: './results/')." << std::endl;
    std::cout << "  --logs, -L: Enable logging of results (default: disabled)." << std::endl;
}

int processInput(int argc, const char *argv[]) {
    for (int i = 1; i < argc; ++i) {
        const char *arg = argv[i];

        if (strcmp(arg, "--help") == 0 || strcmp(arg, "-h") == 0) {
            // Print help and exit.
            printHelp();
            return 0;
        } else if (strncmp(arg, "--input_mode=", 13) == 0 || strncmp(arg, "-I=", 3) == 0) {
            // Set the execution type.
            const char *value = strchr(arg, '=') + 1;

            if (strcmp(value, "random") == 0) {
                // Random initialisation mode.
                INIT_MODE = "random";
            } else if (strcmp(value, "input") == 0) {
                // Input initialisation mode.
                INIT_MODE = "input";
            } else {
                // Invalid execution type.
                std::cout << "Invalid argument for initialization mode. Please use either 'random' or 'input'." << std::endl;
                return 1;
            }
        } else if ((INIT_MODE == "random") && (strncmp(arg, "--num_points=", 13) == 0 || strncmp(arg, "-N=", 3)) == 0) {
            // Set the number of points.
            NUM_POINTS = atoi(strchr(arg, '=') + 1);
        } else if ((INIT_MODE == "input") && (strncmp(arg, "--file_path=", 12) == 0 || strncmp(arg, "-F=", 3)) == 0) {
            // Set the input file path.
            FILE_PATH = strchr(arg, '=') + 1;
        } else if (strncmp(arg, "--num_clusters=", 15) == 0 || strncmp(arg, "-K=", 3) == 0) {
            // Set the number of clusters.
            NUM_CLUSTERS = atoi(strchr(arg, '=') + 1);
        } else if (strncmp(arg, "--dimensions=", 13) == 0 || strncmp(arg, "-D=", 3) == 0) {
            // Set the number of dimensions.
            DIMENSIONS = atoi(strchr(arg, '=') + 1);
        } else if (strncmp(arg, "--execution_type=", 16) == 0 || strncmp(arg, "-E=", 3) == 0) {
            // Set the execution type.
            const char *value = strchr(arg, '=') + 1;

            if (strcmp(value, "sequential") == 0) {
                // Sequential execution type.
                EXECUTION_TYPE = "sequential";
            } else if (strcmp(value, "parallel") == 0) {
                // Parallel execution type.
                EXECUTION_TYPE = "parallel";
            } else {
                // Invalid execution type.
                std::cout << "Invalid argument for execution type. Please use either 'parallel' or 'sequential'." << std::endl;
                return 1;
            }
        } else if ((EXECUTION_TYPE == "parallel") && (strncmp(arg, "--num_threads=", 14) == 0 || strncmp(arg, "-T=", 3) == 0)) {
            // Set the number of threads for parallel execution.
            NUM_THREADS = atoi(strchr(arg, '=') + 1);
        } else if (strncmp(arg, "--base_path=", 12) == 0 || strncmp(arg, "-B=", 3) == 0) {
            if(arg[strlen(arg)-1] == '\\') {
                // Set the base path for the results.
                BASE_PATH = strchr(arg, '=') + 1;
            } else {
                // Invalid base path.
                std::cout << "Invalid argument for base path. Please specify a valid base path." << std::endl;
                return 1;
            }
        } else if (strcmp(arg, "--log") == 0 || strcmp(arg, "-L") == 0) {
            // Enable logging of results.
            LOG = true;
        } else {
            std::cout << "Invalid argument: " << arg << ". Use '--help' or '-h' for usage instructions." << std::endl;
            return 1;
        }
    }

    if (INIT_MODE == "" || (INIT_MODE == "random" && NUM_POINTS < 1) || (INIT_MODE == "input" && FILE_PATH == "") || NUM_CLUSTERS < 1 || DIMENSIONS < 1 || EXECUTION_TYPE == "" || (EXECUTION_TYPE == "parallel" && NUM_THREADS < 1)) {
        std::cout << "Please specify valid values for required parameters." << std::endl;
        return 1;
    }

    return 0;
}

int main(int argc, const char *argv[]) {
    // Process the input.
    if (processInput(argc, argv) != 0) {
        return 1;
    }


    // Run the algorithm.
    if (EXECUTION_TYPE == "sequential") {
        if (INIT_MODE == "random") {
            Sequential::KMeans(NUM_POINTS, NUM_CLUSTERS, DIMENSIONS).run(BASE_PATH, LOG);
        } else {
            Sequential::KMeans(FILE_PATH, NUM_CLUSTERS).run(BASE_PATH, LOG);
        }
    } else {
        if (INIT_MODE == "random") {
            Parallel::KMeans(NUM_POINTS, NUM_CLUSTERS, DIMENSIONS, NUM_THREADS).run(BASE_PATH, LOG);
        } else {
            Parallel::KMeans(FILE_PATH, NUM_CLUSTERS, NUM_THREADS).run(BASE_PATH, LOG);
        }
    }

    return 0;
}