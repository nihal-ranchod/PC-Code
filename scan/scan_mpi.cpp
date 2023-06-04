#include <iostream>
#include <vector>
#include <cmath>
#include <mpi.h>
#include <chrono>

void upsweep(std::vector<int>& arr, int d) {
    int n = arr.size();
    int power = 1 << (d + 1);
    int stride = 1 << d;

    #pragma omp parallel for
    for (int k = 0; k < n; k += power) {
        int idx = k + stride - 1;
        arr[idx] += arr[idx - stride];
    }
}

void downsweep(std::vector<int>& arr, int d) {
    int n = arr.size();
    int power = 1 << (d + 1);
    int stride = 1 << d;

    #pragma omp parallel for
    for (int k = 0; k < n; k += power) {
        int idx = k + stride - 1;
        int temp = arr[idx];
        arr[idx] += arr[idx - stride];
        arr[idx - stride] = temp;
    }
}

void parallelScan(std::vector<int>& arr, int rank, int size) {
    int n = arr.size();
    int numThreads = omp_get_max_threads();

    // Perform upsweep
    for (int d = 0; (1 << d) < n; ++d) {
        #pragma omp parallel for num_threads(numThreads)
        for (int k = rank; k < n; k += size) {
            int index = (k + 1) * (1 << (d + 1)) - 1;
            if (index < n) {
                arr[index] += arr[index - (1 << d)];
            }
        }
    }

    // Set root node to zero
    arr[n - 1] = 0;

    // Perform downsweep
    for (int d = static_cast<int>(log2(n)) - 1; d >= 0; --d) {
        #pragma omp parallel for num_threads(numThreads)
        for (int k = rank; k < n; k += size) {
            int index = (k + 1) * (1 << (d + 1)) - 1;
            if (index < n) {
                int temp = arr[index - (1 << d)];
                arr[index - (1 << d)] = arr[index];
                arr[index] += temp;
            }
        }
    }
}

int main(int argc, char* argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N;
    if (rank == 0) {
        std::cout << "Enter the number of elements: ";
        std::cin >> N;
    }

    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    std::vector<int> arr(N);

    if (rank == 0) {
        std::cout << "Enter the elements:\n";
        for (int i = 0; i < N; ++i) {
            std::cin >> arr[i];
        }
    }

    MPI_Bcast(arr.data(), N, MPI_INT, 0, MPI_COMM_WORLD);

    std::cout << "Process " << rank << " - Original Array: ";
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    parallelScan(arr, rank, size);
    auto end = std::chrono::high_resolution_clock::now();
    double runtime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;

    std::cout << "Process " << rank << " - Scan Result: ";
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    MPI_Finalize();

    if (rank == 0) {
        std::cout << "Runtime: " << runtime << " milliseconds" << std::endl;
    }

    return 0;
}
