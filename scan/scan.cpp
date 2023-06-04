#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include "main.h"

using namespace std;

std::vector<int> scan_serial(std::vector<int> input) {
    std::vector<int> output(input.size());
    output[0] = input[0];

    for (int i = 1; i < input.size(); i++) {
        output[i] = input[i] + output[i - 1];
    }

    return output;
}
