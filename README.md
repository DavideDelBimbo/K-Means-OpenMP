# K-Means-OpenMP
This repository contains a C++ implementation of the K-Means clustering algorithm, optimized for parallel execution through OpenMP. The integration of OpenMP for parallelization empowers significant performance enhancements, particularly on multi-core processors, when compared to sequential implementations.

## Table of Contents
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Usage](#usage)
- [Results](#results)
- [License](#license)

## Prerequisites
Before you get started, make sure you have the following dependencies installed:

- C++ compiler with OpenMP support (e.g. g++).
- The OpenMP library.
- [Gnuplot](http://www.gnuplot.info/).
- [ImageMagick](https://imagemagick.org/).

## Installation
Follow these steps to set up and compile the code:
1. Clone this repository to your local machine using the following command:
<p align="center"><code>git clone DavideDelBimbo/K-Means-OpenMP</code></p>

2. Navigate to the project directory:
<p align="center"><code>cd K-Means-OpenMP</code></p>

3. Modify the parameters in `params.h` as needed to customize the behavior of the K-Means algorithm.

4. Compile the code using g++ with OpenMP support:
<p align="center"><code>g++ main.cpp -o kmean -fopenmp</code></p>

## Usage
To execute the code, use the following command:
<p align="center"><code>./kmean --input_mode [--num_points, --file_path] --num_clusters [--dimensions] --execution_type [--num_threads] --base_path [--logs]</code></p>

Where:
- `--input_mode`: The initialization mode of points (use either 'random' or 'input').
- `--num_points` (required only with `<init_mode> = 'random'`): The number of points to generate.
- `--file_path` (required only with `<init_mode> = 'input'`): Path to dataset with points coordinates to generate.
- `--num_clusters`: The number of clusters to generate.
- `--dimensions` (required only with `<init_mode> = 'random'`): The number of dimensions for each data point.
- `--execution_type`: The execution type (use either 'parallel' or 'sequential').
- `--num_threads` (required only with `<execution_type> = 'parallel'`): The number of threads to use for parallel execution.
- `--base_path`: The base path for the results.
- `--logs` (optional): If provided, it will generate a GIF animation of the execution (note that this may affect execution times).

For example:
<p align="center"><code>./kmean --init_mode='random' --num_points=100000 --num_clusters=10 --dimensions=2 --execution_type=parallel --num_threads=3 --base_path='./results/' --logs</code></p>
<p align="center"><code>./kmean --init_mode='input' --file_path=datasets/dataset_100K.csv --num_clusters=4 --execution_type=sequential --base_path='./results/'</code></p>

## Results
The results obtained from running the K-Means clustering algorithm using OpenMP can be found in <a href="https://github.com/DavideDelBimbo/K-Means-OpenMP/blob/main/report/report.pdf" target="_blank">report</a> file. The results may include information such as the final cluster assignments, execution times and any relevant statistics.

Before diving into the results, you may want to take a moment to configure the parameters in `params.h` to customize the behavior of the K-Means algorithm according to your specific needs and datasets.

## License
This project is licensed under the <a href="https://github.com/DavideDelBimbo/K-Means-OpenMP/blob/main/LICENSE" target="_blank">MIT</a> License.