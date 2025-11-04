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

bool HashTable::insert(const string& key) {
    size_t index = indexFor(key);
    vector<string>& chain = buckets_[index];

    for (size_t i = 0; i < chain.size(); i++) {
        if (chain[i] == key) {
            return false;
        } 
    }

    chain.push_back(key);
    size_ += 1;
    return true;
}

bool HashTable::contains(const string& key) const {
    size_t index = indexFor(key);
    const vector<string>& chain = buckets_[index];

    for (size_t i = 0; i < chain.size(); i++) {
        if (chain[i] == key) {
            return true;
        }
    }

    return false;
}

bool HashTable::erase(const string& key) {
    size_t index = indexFor(key);
    vector<string>& chain = buckets_[index];

    for (size_t i = 0; i < chain.size(); i++) {
        if (chain[i] == key) {
            if (i+1 != chain.size()) {
                string temp = chain.back();
                chain[i] = temp;
            }

            chain.pop_back();

            size_ -= 1;

            return true;
        }
    }
    return false;
}