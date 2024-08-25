#include <iostream>
#include <cstdint>
#include <vector>
#include <thread>
#include <future>
#include <fstream>
#include <algorithm>

using namespace std;

int n, m, num_threads;
vector<uint64_t> subsets;
uint64_t one = static_cast<uint64_t>(1);

uint64_t solve(int index, uint64_t current);

int main(int argc, char *argv[]) {
    
    num_threads = atoi(argv[2]);

    
    cin >> n >> m;

    subsets.resize(m);
    for (int i = 0; i < m; i++) {
        int p, temp;
        cin >> p;
        for (int j = 0; j < p; j++) {
            cin >> temp;
            subsets[i] |= (one << temp);
        }
    }

    
    vector<future<uint64_t>> futures;

    for (int i = 0; i < m; ++i) {
        futures.push_back(async(launch::async, [i] { return solve(i + 1, subsets[i]); }));
    }

    uint64_t result = 0;
    for (auto& future : futures) {
        result += future.get();
    }

    cout << result << endl;

    return 0;
}

uint64_t solve(int index, uint64_t current) {
    if (index == m) {
        return (current == (one << n) - 1) ? 1 : 0;
    } else {
        return solve(index + 1, current) + solve(index + 1, current | subsets[index]);
    }
}





