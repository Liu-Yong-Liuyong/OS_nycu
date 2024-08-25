#include <iostream>
#include <vector>
#include <unordered_map>
#include <list>
#include <algorithm>
#include <deque>
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

    // Initialize the cache as a 2D vector with -1 indicating empty slots
    vector<vector<int>> cache(numSets, vector<int>(k, -1));

    // Initialize a vector to keep track of whether a cache slot is occupied
    vector<vector<bool>> isOccupied(numSets, vector<bool>(k, false));

    // Initialize a map to keep track of the LRU order for each set
    unordered_map<int, list<int>> lruMap;

    // Variable to count cache misses
    int cacheMisses = 0;

    // Traverse the reference data stream
    for (int i = 0; i < dataSize; ++i) {
        int data = referenceData[i];

        // Calculate the set index
        int setIndex = i % numSets;

        // Check if data is in the cache
        bool found = false;

        // Iterate over all sets in the cache
        for (int setIndex = 0; setIndex < numSets; ++setIndex) {
            // Check if data is in the cache
            auto it = find(cache[setIndex].begin(), cache[setIndex].end(), data);
            if (it != cache[setIndex].end()) {
                // Data is in the cache, update the LRU order
                lruMap[setIndex].remove(data);
                lruMap[setIndex].push_back(data);
                found = true;
                break;  // Exit the loop if data is found
            }
        }
        if (!found) {
            // Data is not in the cache, count it as a cache miss
            ++cacheMisses;

            // Check if the set is full
            if (lruMap[setIndex].size() == k) {
                // Set is full, apply LRU block replacement policy

                // Find the least recently used block
                int lruBlock = lruMap[setIndex].front();
                lruMap[setIndex].pop_front();

                // Find the index of the least recently used block in the cache
                auto lruIt = find(cache[setIndex].begin(), cache[setIndex].end(), lruBlock);
                int lruIndex = distance(cache[setIndex].begin(), lruIt);

                // Replace the least recently used block with the new block
                cache[setIndex][lruIndex] = data;
                isOccupied[setIndex][lruIndex] = true;
            } else {
                // Set is not full, find the first empty place in the cache
                auto emptyIt = find(isOccupied[setIndex].begin(), isOccupied[setIndex].end(), false);
                int emptyIndex = distance(isOccupied[setIndex].begin(), emptyIt);
                cache[setIndex][emptyIndex] = data;
                isOccupied[setIndex][emptyIndex] = true;
            }

            // Update the LRU order for the set
            lruMap[setIndex].remove(data);
            lruMap[setIndex].push_back(data);
        }
    }


    // Print the total number of cache misses
    cout << "Total Cache Misses:" << cacheMisses << endl;

    return 0;
}
