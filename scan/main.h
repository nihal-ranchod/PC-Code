#pragma once

#include <vector>
#include <cstddef>

using namespace std;

std::vector<int> scan_serial(std::vector<int> input);
std::vector<int> scan_omp(std::vector<int>& input);