#include "trie.h"
#include "hashtable.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void printSeparator() {
    cout << endl << string(50, '=') << endl << endl;
}

void demonstrateTrie() {
    cout << "TRIE DEMONSTRATION" << endl;
    printSeparator();

    Trie trie;

    // Insert words
    cout << "Inserting words: cat, car, card, care, careful, dog, dodge" << endl;
    trie.insert("cat");
    trie.insert("car");
    trie.insert("card");
    trie.insert("care");
    trie.insert("careful");
    trie.insert("dog");
    trie.insert("dodge");

    // Search for words
    cout << endl << "Searching for words:" << endl;
    vector<string> searchWords = {"cat", "car", "can", "care", "careful", "dog", "dot"};
    for (const string& word : searchWords) {
        cout << "  '" << word << "': " << (trie.search(word) ? "Found" : "Not found") << endl;
    }

    // Check prefixes
    cout << endl << "Checking prefixes:" << endl;
    vector<string> prefixes = {"ca", "car", "do", "dod", "z"};
    for (const string& prefix : prefixes) {
        cout << "  '" << prefix << "': " << (trie.startsWith(prefix) ? "Exists" : "Does not exist") << endl;
    }

    // Remove words
    cout << endl << "Removing 'car' and 'dog':" << endl;
    trie.remove("car");
    trie.remove("dog");

    cout << "After removal:" << endl;
    cout << "  'car': " << (trie.search("car") ? "Found" : "Not found") << endl;
    cout << "  'card': " << (trie.search("card") ? "Found" : "Not found") << endl;
    cout << "  'dog': " << (trie.search("dog") ? "Found" : "Not found") << endl;
    cout << "  'dodge': " << (trie.search("dodge") ? "Found" : "Not found") << endl;
}

void demonstrateHashTable() {
    cout << "HASH TABLE DEMONSTRATION" << endl;
    printSeparator();

    HashTable hashTable(16);

    cout << "Initial bucket count: " << hashTable.bucketCount() << endl;
    cout << "Initial size: " << hashTable.size() << endl << endl;

    // Insert elements
    cout << "Inserting elements:" << endl;
    vector<string> elements = {"apple", "banana", "cherry", "date", "elderberry"};
    for (const string& elem : elements) {
        bool inserted = hashTable.insert(elem);
        cout << "  '" << elem << "': " << (inserted ? "Inserted" : "Already exists") << endl;
    }

    // Try inserting duplicate
    cout << endl << "Trying to insert duplicate 'apple': "
         << (hashTable.insert("apple") ? "Inserted" : "Already exists") << endl;

    cout << endl << "Current size: " << hashTable.size() << endl;

    // Check containment
    cout << endl << "Checking containment:" << endl;
    vector<string> checkElements = {"apple", "banana", "grape", "cherry", "mango"};
    for (const string& elem : checkElements) {
        cout << "  '" << elem << "': " << (hashTable.contains(elem) ? "Present" : "Not present") << endl;
    }

    // Erase elements
    cout << endl << "Erasing 'banana' and 'date':" << endl;
    cout << "  'banana': " << (hashTable.erase("banana") ? "Removed" : "Not found") << endl;
    cout << "  'date': " << (hashTable.erase("date") ? "Removed" : "Not found") << endl;

    // Try erasing non-existent element
    cout << "  'mango': " << (hashTable.erase("mango") ? "Removed" : "Not found") << endl;

    cout << endl << "Final size: " << hashTable.size() << endl;

    // Check after erasure
    cout << endl << "Checking after erasure:" << endl;
    cout << "  'banana': " << (hashTable.contains("banana") ? "Present" : "Not present") << endl;
    cout << "  'cherry': " << (hashTable.contains("cherry") ? "Present" : "Not present") << endl;
}

int main() {
    cout << endl << "*** DATA STRUCTURES DEMONSTRATION ***" << endl;

    // Demonstrate Trie
    demonstrateTrie();
    printSeparator();

    // Demonstrate HashTable
    demonstrateHashTable();
    printSeparator();

    cout << "Program complete!" << endl;
    return 0;
}
