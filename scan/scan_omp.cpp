#include <vector>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <chrono>
#include <stddef.h>
#include <omp.h>

using namespace std;

#define pow2(x) std::pow(2, x)

void up_sweep(std::vector<int>& input) {
    size_t max_d = static_cast<size_t>(std::log2(input.size()));

    for (size_t d = 0; d < max_d; ++d) {

        size_t p2_0 = static_cast<size_t>(pow2(d));
        size_t p2_1 = static_cast<size_t>(pow2(d + 1));

        #pragma omp for
        for (size_t i = 0; i < input.size(); i += p2_1) {
            input[i + p2_1 - 1] += input[i + p2_0 - 1];
        }
    }
}

void down_sweep(std::vector<int>& input) {
    size_t max_d = static_cast<size_t>(std::log2(input.size()));
    input[input.size() - 1] = 0;

    for (size_t d = max_d; d-- > 0; ) {
        size_t p2_0 = static_cast<size_t>(pow2(d));
        size_t p2_1 = static_cast<size_t>(pow2(d + 1));

        #pragma omp for
        for (size_t i = 0; i < input.size(); i += p2_1) {
            int32_t temp = input[i + p2_0 - 1];
            input[i + p2_0 - 1] = input[i + p2_1 - 1];
            input[i + p2_1 - 1] += temp;
        }
    }

}

std::vector<int> scan_omp(std::vector<int>& input) {

    // Either set the number of threads to the min of number of elements or number of available threads
    size_t num_threads = input.size() < omp_get_max_threads() ? input.size() : omp_get_max_threads();
    omp_set_num_threads(num_threads);

    std::vector<int> result(input);
    std::vector<int> sum(num_threads);
    size_t thread_size = result.size() / num_threads; // Length of sublist in thread

    #pragma omp parallel shared(result, sum, input)
    { 
        size_t thread_id = omp_get_thread_num();
        size_t start_idx = thread_id * thread_size; // Starting index of the sublist

        for (size_t j = 1; j < thread_size; ++j) {
            result[start_idx + j] = result[start_idx + j] + result[start_idx + j - 1];
        }

        sum[thread_id] = result[start_idx + thread_size - 1];
    }

    // auto sweep_start = std::chrono::high_resolution_clock::now();
    up_sweep(sum);
    down_sweep(sum);

    // auto apply_start = std::chrono::high_resolution_clock::now();
    // Apply the sum to all the thread sublists    
    #pragma omp parallel shared(sum, result)
    {
        size_t thread_id = omp_get_thread_num();
        size_t start_idx = thread_id * thread_size; // Starting index of the sublist

        for (size_t i = 0; i < thread_size; ++i) {
            result[start_idx + i] += sum[thread_id];
        }
    }

    return result;
}