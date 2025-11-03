#include "hashtable.h"
#include <functional>
#include <utility>

using namespace std;

HashTable::HashTable(size_t bucketCount) {
    if (bucketCount == 0) {
        bucketCount = 8;
    }

    buckets_.resize(bucketCount);
    size_ = 0;
}

size_t HashTable::indexFor(const string& key) const {
    hash<string> hasher;
    size_t hashValue = hasher(key);
    size_t index = hashValue % buckets_.size();
    return index;
}