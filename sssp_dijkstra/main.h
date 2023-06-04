#pragma once

#include <vector>
#include <cstddef>

using namespace std;

void dijkstra_serial(vector<vector<int>>& graph, int source);
void dijkstra_omp(vector<vector<int>>& graph, int source);