#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <vector>
#include <cstddef>

using std::size_t;
using std::string;
using std::vector;

class HashTable {
    public:
    explicit HashTable(size_t bucketCount = 32768);

    bool insert(const string& key);
    bool contains(const string& key) const;
    bool erase(const string& key);

    size_t size() const {
        return size_;
    }

    size_t bucketCount() const {
        return buckets_.size();
    }

    private:
    vector<vector<string>> buckets_;
    size_t size_;
    size_t indexFor(const string& key) const;
};

#endif