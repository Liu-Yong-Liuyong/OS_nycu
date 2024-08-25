#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;

int main() {
    int cache_blk;
    cin >> cache_blk;
    int datanum;
    cin >> datanum;
    vector<int> referenceData(datanum);
    for (int i = 0; i < datanum; ++i) {
        cin >> referenceData[i];
    }

    // Initialize the cache as an array with -1 indicating empty slots
    vector<int> cache(cache_blk, -1);

    // Initialize a set to keep track of data present in the cache
    unordered_set<int> cacheSet;

    int cacheMisses = 0;
    for (int i = 0; i < datanum; ++i) {
        int data = referenceData[i];

        // Check if data is in the cache
        auto it = cacheSet.find(data);
        if (it == cacheSet.end()) {
            // Data is not in the cache, count it as a cache miss
            ++cacheMisses;
            cacheSet.erase(cache[i%cache_blk]);
            cache[i%cache_blk] = data;
            cacheSet.insert(data);
        }
    }
    cout << "Total Cache Misses:" << cacheMisses << endl;

    return 0;
}
