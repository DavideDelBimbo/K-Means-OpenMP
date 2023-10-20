#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <direct.h>

#include "point.h"
#include "centroid.h"


void log(int iteration, const std::vector<Point>& points, const std::vector<Centroid>& centroids, const double& execution_time, bool isParallel) {
    // Set the folder where the logs are stored.
    std::string folder = !isParallel ? "results\\sequential\\log\\" : "results\\parallel\\log\\";
    // Create folder.
    _mkdir(folder.c_str());

    // Output files.
    std::ofstream output_points, output_clusters, output_time;
    

    // Log the results of points at each iteration.
    if(iteration==0) {
        output_points.open(folder + "log_points.csv");
        // Log the header.
        output_points << "iteration,x_coordinate,y_coordinate,cluster\n";
    } else {
        output_points.open(folder + "log_points.csv", std::ios_base::app);
    }

    for (int i=0; i<points.size(); i++) {
        output_points << iteration << "," << points[i].x << "," << points[i].y << "," << points[i].cluster << "\n";
    }
    output_points.close();


    // Log the results of clusters at each iteration.
    if(iteration==0) {
        output_clusters.open(folder + "log_clusters.csv");
        // Log the header.
        output_clusters << "iteration,x_coordinate,y_coordinate,cluster\n";
    } else {
        output_clusters.open(folder + "log_clusters.csv", std::ios_base::app);
    }
    
    for (int i=0; i<centroids.size(); i++) {
        output_clusters << iteration << "," << centroids[i].x << "," << centroids[i].y << "," << i << "\n";
    }
    output_clusters.close();
    

    // Log the time at each iteration.
    if(iteration==0) {
        output_time.open(folder + "log_time.csv");
        // Log the header.
        output_time << "iteration,time(s)\n";
    } else {
        output_time.open(folder + "log_time.csv", std::ios_base::app);
    }
    
    output_time << iteration << "," << execution_time << "\n";
    output_time.close();
}

#endif // UTILS_H