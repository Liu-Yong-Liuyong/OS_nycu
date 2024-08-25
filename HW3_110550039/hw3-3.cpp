#include <iostream>
#include <fstream>
#include <vector>
#include <thread>

using namespace std;

int n, global_count = 0;

bool is_prime(int num) {
    if (num == 1) return false;

    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) {
            return false;
        }
    }
    return true;
}

// Function to count prime numbers in a range [start, end)
int count_primes_in_range(int start, int end) {
    int count = 0;
    for (int i = start; i < end; i++) {
        if (is_prime(i)) {
            count++;
        }
    }
    return count;
}

int main(int argc, char* argv[]) {

    // Parse command line arguments
    int num_threads = stoi(argv[2]);
    cin>>n;
    if (n < num_threads) {
        num_threads = n;
    }

    // Specify the number of threads
    int chunk_size = n / num_threads;

    vector<thread> threads;
    vector<int> thread_counts(num_threads, 0);

    // Split the work among threads
    for (int i = 0; i < num_threads; i++) {
        int start = i * chunk_size + 1;
        int end = (i == num_threads - 1) ? n + 1 : (i + 1) * chunk_size + 1;

        threads.emplace_back([start, end, &thread_counts, i]() {
            thread_counts[i] = count_primes_in_range(start, end);
        });
    }

    // Join threads
    for (auto& thread : threads) {
        thread.join();
    }

    // Aggregate results
    for (int count : thread_counts) {
        global_count += count;
    }

    cout << global_count << endl;

    return 0;
}
