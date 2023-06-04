#include <iostream>
#include <stdlib.h>
#include <vector>
#include <chrono>
#include <array>
#include <math.h>
#include "main.h"
#include <stdio.h>
#include <random>
using namespace std;

void checkCorrect(std::vector<int>& data) {
    bool isValid = true;
    for (int i = 0; i < data.size() - 1; i++) {
        if (data[i] > data[i + 1]) {
            isValid = false;
            break;
        }
    }

    if (isValid) {
        std::cout << "Valid" << std::endl;
    } else {
        std::cout << "Invalid" << std::endl;
    }
}

int main(){

    std::vector<int> randomList = {2,1,4,0,3,7,6,3}; 
    // int lenRandomList = pow(2,4);

    // for (int i = 0; i < lenRandomList; i++){
    //     int randomNum = rand();
    //     randomList.push_back(randomNum);
    // }

    //Serial Scan Implementation:
    auto serial_start = std::chrono::high_resolution_clock::now();
    std::vector<int> output_serial = scan_serial(randomList);
    auto serial_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> serial_time = serial_end - serial_start;
    
    printf("Serial Execution time = %f ms\n", serial_time.count());
    for (int i = 0; i < output_serial.size(); i++) {
        std::cout <<output_serial[i] << " ";
    }
    checkCorrect(output_serial);

    //Parallel Scan openMP Implementation:
    auto omp_start = std::chrono::high_resolution_clock::now();
    std::vector<int> omp_solution = scan_omp(randomList);
    auto omp_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> omp_time = omp_end - omp_start;

    printf("OMP Execution time = %f ms\n", omp_time.count());
    for (int i = 0; i < omp_solution.size(); i++) {
        std::cout << omp_solution[i] << " ";
    }
    checkCorrect(omp_solution);
}

