#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <limits>

using namespace std;

int main() {
    // Read the number of cache lines
    int numCacheLines;
    cin >> numCacheLines;

    // Read K, the number of cache lines/blocks in a set
    int k;
    cin >> k;

    // Calculate the number of sets
    int numSets = numCacheLines / k;

    // Read the size of the reference data stream
    int dataSize;
    cin >> dataSize;

    // Read the reference data stream
    vector<int> referenceData(dataSize);
    for (int i = 0; i < dataSize; ++i) {
        cin >> referenceData[i];
    }

    // Initialize the cache as a 2D vector
    vector<vector<int>> cache(numSets, vector<int>(k, -1));

    // Initialize a map to keep track of the frequency of each block
    unordered_map<int, int> frequencyMap;

    // Variable to count cache misses
    int cacheMisses = 0;

    // Traverse the reference data stream
    for (int i = 0; i < dataSize; ++i) {
        int data = referenceData[i];

        // Calculate the set index
        int setIndex = i % numSets;

        // Check if data is in the cache
        //auto it = find(cache[setIndex].begin(), cache[setIndex].end(), data);

        // Check if data is in the cache
        bool found = false;

        // Iterate over all sets in the cache
        for (int setIndex = 0; setIndex < numSets; ++setIndex) {
            // Check if data is in the cache
            auto it = find(cache[setIndex].begin(), cache[setIndex].end(), data);
            if (it != cache[setIndex].end()) {
                ++frequencyMap[data];
                found = true;
                break;  // Exit the loop if data is found
            }
        }
        if (!found) {
            // Data is not in the cache, count it as a cache miss
            ++cacheMisses;

            // Check if the set is full
            if (cache[setIndex].size() == k) {
                // Set is full, apply LFU block replacement policy

                // Find the block with the lowest frequency
                int minFrequency = numeric_limits<int>::max();
                int minFrequencyBlock = -1;

                for (int block : cache[setIndex]) {
                    if (frequencyMap[block] < minFrequency) {
                        minFrequency = frequencyMap[block];
                        minFrequencyBlock = block;
                    }
                }

                // Replace the block with the lowest frequency with the new block
                auto replaceIt = find(cache[setIndex].begin(), cache[setIndex].end(), minFrequencyBlock);
                *replaceIt = data;
            } else {
                // Set is not full, find the first empty place in the cache
                auto emptyIt = find(cache[setIndex].begin(), cache[setIndex].end(), -1);
                *emptyIt = data;
            }

            // Update the frequency of the new block
            ++frequencyMap[data];
        }
    }

    // Print the total number of cache misses
    cout << "Total Cache Misses:" << cacheMisses << endl;

    return 0;
}
