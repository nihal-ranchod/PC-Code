#include <iostream>
#include <vector>
#include <omp.h>

std::vector<int> parallelScan(std::vector<int> arr) {
    std::vector<int> partial;
    std::vector<int> temp;
    int num_threads, work;
    int n = arr.size();

    #pragma omp parallel default(none) private(num_threads, work, partial, temp) shared(arr, n)
    {
        num_threads = omp_get_num_threads();
        partial.resize(num_threads);
        temp.resize(num_threads);
        work = n / num_threads + 1; /*sets length of sub-arrays*/

        int mynum = omp_get_thread_num();

        /*calculate prefix-sum for each subarray*/
        for (int i = work * mynum + 1; i < work * mynum + work && i < n; i++)
            arr[i] += arr[i - 1];

        partial[mynum] = arr[work * mynum + work - 1];

        #pragma omp barrier

        /*calculate prefix sum for the array that was made from last elements of each of the previous sub-arrays*/
        for (int i = 1; i < num_threads; i <<= 1) {
            if (mynum >= i)
                temp[mynum] = partial[mynum] + partial[mynum - i];

            #pragma omp barrier

            #pragma omp single
            {
                for (int j = 1; j < num_threads; j++)
                    partial[j] = temp[j];
            }
        }

        /*update original array*/
        for (int i = work * mynum; i < std::min(work * mynum + work, n); i++)
            arr[i] += partial[mynum] - arr[std::min(work * mynum + work - 1, n - 1)];
    }
    return arr;

}