#ifndef K_UTILS_H
#define K_UTILS_H

#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>
#include <sys/stat.h>
#include <omp.h>

#include "params.h"


struct FolderPaths {
    std::string outputFolder;
    std::string logFolder;
    std::string imagesFolder;
};

/*
    * Function to create necessary folders and retrieve folder paths.
    *
    * @param basePath The base path where the folders are created.
    *
    * @return The paths of the folders.
*/
inline FolderPaths create_folders(const std::string& basePath) {
    struct stat buffer;
    FolderPaths paths;

    // Set the folder where the results are stored.
    paths.outputFolder = basePath;
    // Create folder if not exists.
    if (stat(paths.outputFolder.c_str(), &buffer) != 0) {
        system(("mkdir " + paths.outputFolder).c_str());
    }

    // Set the folder where the logs are stored.
    paths.logFolder = paths.outputFolder + "logs\\";
    // Create folder if not exists.
    if (stat(paths.logFolder.c_str(), &buffer) != 0) {
        system(("mkdir " + paths.logFolder).c_str());
    }

    // Set the folder where the images are stored.
    paths.imagesFolder = paths.outputFolder + "images\\";
    // Create folder if not exists.
    if (stat(paths.imagesFolder.c_str(), &buffer) != 0) {
        system(("mkdir " + paths.imagesFolder).c_str());
    }

    return paths;
}

/*
    * Function to log data to files.
    *
    * @param iteration: Current iteration.
    * @param paths: Paths of the folders.
    * @param executionType: Type of the execution (sequential or parallel).
    * @param dataType: Type of the data (points or centroids).
    * @param coordinates: Coordinates of the data.
    * @param ids: Identifiers of the data.
*/
inline void log_data(int iteration, const FolderPaths& paths, std::string executionType, std::string dataType, const std::vector<std::vector<double>>& coordinates, const std::vector<int>& ids) {
    // Convert to lowercase the execution type and the data type strings.
    std::transform(executionType.begin(), executionType.end(), executionType.begin(), ::tolower);
    std::transform(dataType.begin(), dataType.end(), dataType.begin(), ::tolower);


    // Set the file name.
    std::stringstream ss;
    ss << std::setw(3) << std::setfill('0') << (iteration + 1);
    std::string outputName = ss.str();


    // Output file.
    std::ofstream outfile(paths.logFolder + dataType + "_" + outputName + ".txt");
    for (int i = 0; i < coordinates.size(); i++) {
        // Log the coordinates of the data.
        for (double value : coordinates[i]) {
            outfile << value << " ";
        }
        
        // Log the identifier of the data.
        outfile << ids[i] << std::endl;
    }
    outfile.close();
}

/*
    * Function to plot data.
    *
    * @param iteration: Current iteration.
    * @param paths: Paths of the folders.
    * @param executionType: Type of the execution (sequential or parallel).
    * @param N: Number of points.
    * @param K: Number of clusters.
    * @param dimensions: Dimensions of the data.
*/
inline void plot_data(int iteration, const FolderPaths& paths, std::string executionType, int N, int K, int dimensions) {
    // Convert to lowercase the execution type string.
    std::transform(executionType.begin(), executionType.end(), executionType.begin(), ::tolower);


    // Set the file name.
    std::stringstream ss;
    ss << std::setw(3) << std::setfill('0') << (iteration + 1);
    std::string outputName = ss.str();


    // Vector of colors.
    std::vector<std::string> colors = {
        "0 '#FE0000'", "1 '#0000FE'", "2 '#5600E2'", "3 '#6C5EE2'", "4 '#00FEFE'",
        "5 '#80FE96'", "6 '#00FE00'", "7 '#FEFE00'", "8 '#FEA820'", "9 '#FE74FE'"
    };

    // Title of the plot.
    std::string capitilizedExecutionType = executionType;
    capitilizedExecutionType[0] = toupper(capitilizedExecutionType[0]);
    std::string title = "K-Means Clustering (" + capitilizedExecutionType + (executionType == "parallel" ? (" - #Threads " + std::to_string(omp_get_max_threads())) : "") + ")";
    
    // Size of the image.
    int size_x = 1000;
    int size_y = 900;


    // Set the gnuplot command.
    std::string gnuplot = "gnuplot -e \"";

    // Set the palette.
    std::string colorsString = "";
    if (K <= colors.size()) {
        for (int j = 0; j < K; j++) {
            colorsString += colors[j];

            if (j < K - 1) {
                colorsString += ", ";
            }
        }

        // Set the palette colors.
        gnuplot += "set palette model RGB defined (" + colorsString + ");";
    }

    // Set the title of the plot.
    gnuplot += "set title '" + title + "';";

    // Set the size of the image.
    gnuplot += "set terminal png size " + std::to_string(size_x) + "," + std::to_string(size_y) + ";";

    // Set the output file.
    gnuplot += "set output '" + paths.imagesFolder + "plot_" + outputName + ".png';";

    if (dimensions == 2) {
        // Set the range of the axes.
        gnuplot += "set xrange[" + std::to_string(0) + ":" + std::to_string(MAX_RANGE) + "];";
        gnuplot += "set yrange[" + std::to_string(0) + ":" + std::to_string(MAX_RANGE) + "];";

        // Plot the points.
        gnuplot += "plot '" + paths.logFolder + "points_" + outputName + ".txt' using 1:2:3 with points pointtype 20 palette notitle,";
        
        // Plot the centroids.
        gnuplot += "'" + paths.logFolder + "centroids_" + outputName + ".txt' using 1:2:(0.08) with circles linewidth 2 linecolor rgb 'black' notitle";
    } else if (dimensions == 3) {
        // Set the range of the axes.
        gnuplot += "set xrange[" + std::to_string(0) + ":" + std::to_string(MAX_RANGE) + "];";
        gnuplot += "set yrange[" + std::to_string(0) + ":" + std::to_string(MAX_RANGE) + "];";
        gnuplot += "set zrange[" + std::to_string(0) + ":" + std::to_string(MAX_RANGE) + "];";

        // Set grid.
        gnuplot += "set xyplane at 0;set grid vertical xtics ytics ztics;set view 60, 30;";

        // Plot the points.
        gnuplot += "splot '" + paths.logFolder + "points_" + outputName + ".txt' using 1:2:3:4 with points pointtype 20 palette notitle,";
        
        // Plot the centroids.
        gnuplot += "'" + paths.logFolder + "centroids_" + outputName + ".txt' using 1:2:3:(0.08) with circles linewidth 2 linecolor rgb 'black' notitle";
    }

    // Close the command.
    gnuplot += "\"";

    // Execute the command.
    system(gnuplot.c_str());
}

/* 
    *Function to convert results into GIF animation
    *
    * @param iterations: Number of iterations.
    * @param executionTimes: Execution time.
    * @param paths: Paths of the folders.
    * @param executionType: Type of the execution (sequential or parallel).
*/
inline void convert_gif(int iterations, double executionTimes, const FolderPaths& paths, std::string executionType) {
    // Convert to lowercase the execution type string.
    std::transform(executionType.begin(), executionType.end(), executionType.begin(), ::tolower);

    
    // Set the file name.
    std::stringstream ss;
    ss << std::setw(3) << std::setfill('0') << (iterations);
    std::string outputName = ss.str();


    // Set the delay.
    double delay = executionTimes / iterations;


    // Set the convert command.
    std::string convert = "convert -delay " + std::to_string(delay) + " -loop 0 " + paths.imagesFolder + "*.png " + paths.outputFolder + executionType + "_" + std::to_string(executionTimes) + "s.gif";

    // Create GIF.
    system(convert.c_str());


    // Move the first and the last image representing the final clustering.
    system(("move " + paths.imagesFolder + "plot_001.png " + paths.outputFolder + " > nul").c_str());
    system(("move " + paths.imagesFolder + "plot_" + outputName + ".png " + paths.outputFolder + " > nul").c_str());


    // Remove logs and images folders
    system(("rmdir /S /Q " + paths.logFolder + " > nul").c_str());
    system(("rmdir /S /Q " + paths.imagesFolder + " > nul").c_str());
}

/*
    * Function to save results to a file.
    *
    * @param iterations: Number of iterations.
    * @param executionTimes: Execution time.
    * @param outputFolder: Folder where the results are stored.
    * @param executionType: Type of the execution (sequential or parallel).
    * @param N: Number of points.
    * @param K: Number of clusters.
    * @param dimensions: Dimensions of the data.
*/ 
inline void save_results(int iterations, double executionTimes, const std::string& outputFolder, std::string executionType, int N, int K, int dimensions) {
    struct stat buffer;
    std::ofstream outfile;


    // Convert to lowercase the execution type string.
    std::transform(executionType.begin(), executionType.end(), executionType.begin(), ::tolower);


    // Create output folder if not exists.
    if (stat(outputFolder.c_str(), &buffer) != 0) {
        system(("mkdir " + outputFolder).c_str());
    }


    // Check if the file exists
    if (stat((outputFolder + "results.txt").c_str(), &buffer) == 0) {
        // File exists, append to existing one
        outfile.open(outputFolder + "results.txt", std::ios_base::app);
    } else {
        // File doesn't exist, create new one with header
        outfile.open(outputFolder + "results.txt");
        outfile << (executionType == "parallel" ? "num_threads " : "") << "num_points num_clusters dimensions execution_time iterations" << std::endl;
    }

    // Save the results.
    outfile << (executionType == "parallel" ? (std::to_string(omp_get_max_threads()) + " ") : "") << N << " " << K << " " << dimensions << " " << executionTimes << " " << iterations << std::endl;
    outfile.close();
}

#endif // K_UTILS_H