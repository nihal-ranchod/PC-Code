#include <iostream>
#include <vector>
#include <limits>

#include "main.h"

using namespace std;

// Structure to represent a vertex and its distance from the source
struct Vertex {
    int index;
    int distance;
};

// Function to perform Dijkstra's algorithm
void dijkstra_serial(vector<vector<int>>& graph, int source) {
    int numVertices = graph.size();

    // Create a vector to store the distances from the source vertex
    vector<int> distances(numVertices, numeric_limits<int>::max());

    // Create a vector to track the visited vertices
    vector<bool> visited(numVertices, false);

    // Create a vector to store the shortest path from the source vertex
    vector<vector<int>> shortestPaths(numVertices);

    // Set the distance of the source vertex to 0
    distances[source] = 0;
    shortestPaths[source].push_back(source);

    // Find shortest path for all vertices
    for (int i = 0; i < numVertices - 1; ++i) {
        // Find the vertex with the minimum distance
        int minDistance = numeric_limits<int>::max();
        int minVertex = -1;

        for (int j = 0; j < numVertices; ++j) {
            if (!visited[j] && distances[j] < minDistance) {
                minDistance = distances[j];
                minVertex = j;
            }
        }

        // Mark the selected vertex as visited
        visited[minVertex] = true;

        // Update the distances of the adjacent vertices
        for (int j = 0; j < numVertices; ++j) {
            if (!visited[j] && graph[minVertex][j] != 0 &&
                distances[minVertex] + graph[minVertex][j] < distances[j]) {
                distances[j] = distances[minVertex] + graph[minVertex][j];
                shortestPaths[j] = shortestPaths[minVertex];
                shortestPaths[j].push_back(j);
            }
        }
    }

    // Print the distances from the source vertex
    cout << "The distance from " << source << " to each vertex is:\n";
    cout << "  v    dist " << source << "->v\n";
    cout << "----   ---------\n";
    for (int i = 1; i < numVertices; ++i) {
        cout << "  " << i << "          " << distances[i] << "\n";
    }

    // Print the shortest path from the source vertex
    cout << "\nThe shortest path from " << source << " to each vertex is:\n";
    cout << "  v     Path " << source << "->v\n";
    cout << "----    ---------\n";
    for (int i = 1; i < numVertices; ++i) {
        cout << "  " << i << ":    ";
        for (int j = 0; j < shortestPaths[i].size(); ++j) {
            cout << shortestPaths[i][j] << " ";
        }
        cout << "\n";
    }
}
