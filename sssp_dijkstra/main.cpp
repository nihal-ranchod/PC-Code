#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <chrono>
#include <array>
#include <stdio.h>
#include "main.h"

using namespace std;

int main(){
    int numVertices, numEdges;
    ifstream inputFile("./input graphs/graph_1.txt");

    if (inputFile.is_open()) {
        // Read the number of vertices and edges from the file
        inputFile >> numVertices >> numEdges;

        // Create an adjacency matrix to represent the graph
        vector<vector<int>> graph(numVertices, vector<int>(numVertices, 0));

        // Read the edges and their weights from the file
        for (int i = 0; i < numEdges; ++i) {
            int source, destination, weight;
            inputFile >> source >> destination >> weight;
            graph[source][destination] = weight;
            graph[destination][source] = weight;
        }

        inputFile.close();

        // Perform Dijkstra's algorithm

        //Sequential Implementation
        auto serial_start = std::chrono::high_resolution_clock::now();
        dijkstra_serial(graph, 0);
        auto serial_end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> serial_time = serial_end - serial_start;
        printf("Serial Execution time = %f ms\n", serial_time.count());

        //OpenMP Implementation
        auto omp_start = std::chrono::high_resolution_clock::now();
        dijkstra_omp(graph, 0);
        auto omp_end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> omp_time = omp_end - omp_start;
        double omp_speedup = serial_time.count() / omp_time.count();
        printf("OMP Execution time = %f ms, OMP Speedup = %lf\n", omp_time.count(), omp_speedup);

    } else {
        cout << "Failed to open the input file.\n";
    }

    return 0;
}