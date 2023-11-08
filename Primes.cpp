#include <iostream>
#include <vector>
#include <thread>
#include <bitset>
#include <chrono>

const int MAX_RANGE = 1000000000;  // Adjust the maximum range as needed, default is large, about 3 seconds on 2020 era 12 core.
const int NUM_THREADS = 48;  // Adjust the number of threads as needed, this can be set higher than the number of cores on your system and can improve performance up to a point.

std::bitset<MAX_RANGE> primeFlags;

void sieveOfEratosthenes(int start, int end) {
    for (int p = 2; p * p <= end; p++) {
        if (primeFlags[p]) {
            for (int i = p * p; i <= end; i += p) {
                primeFlags[i] = false;
            }
        }
    }
}

int main() {
    std::chrono::high_resolution_clock::time_point startTime, endTime;

    primeFlags.set();  // Set all bits to 1 (initially assume all numbers are prime)

    int chunkSize = MAX_RANGE / NUM_THREADS;
    std::vector<std::thread> threads;

    startTime = std::chrono::high_resolution_clock::now();

    for (int i = 2; i * i <= MAX_RANGE; i++) {
        if (primeFlags[i]) {
            for (int j = i * i; j <= MAX_RANGE; j += i) {
                primeFlags[j] = false;
            }
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        int start = i * chunkSize;
        int end = (i == NUM_THREADS - 1) ? MAX_RANGE : (i + 1) * chunkSize - 1;

        threads.emplace_back(sieveOfEratosthenes, start, end);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    int primeCount = 0;
    for (int i = 2; i <= MAX_RANGE; i++) {
        if (primeFlags[i]) {
            primeCount++;
        }
    }
    std::cout << "Found " << primeCount << " primes." << std::endl;
    std::cout << "Time taken: " << duration << " ms" << std::endl;
    // wait for input before exiting
    std::cin.get();
    return 0;
}
