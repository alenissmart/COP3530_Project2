#include "trie.h"
#include "hashtable.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <random>
#include <iomanip>
#include <algorithm>

using namespace std;
using namespace chrono;

class BenchmarkSystem {
private:
    Trie* trie;
    HashTable* hashTable;
    vector<string> allWords;
    bool trieBuilt;
    bool hashTableBuilt;

    // Store results for CSV export
    long long trieBuildTime;
    long long hashBuildTime;
    double trieAvgLookup;
    double hashAvgLookup;
    size_t trieMemory;
    size_t hashMemory;

    size_t estimateTrieMemory() {
        size_t nodeSize = sizeof(TrieNode);
        size_t estimatedNodes = allWords.size() * 5;
        return estimatedNodes * nodeSize;
    }

    size_t estimateHashTableMemory() {
        size_t bucketOverhead = hashTable->bucketCount() * sizeof(vector<string>);
        size_t stringOverhead = hashTable->size() * 50;
        return bucketOverhead + stringOverhead;
    }

    void exportResultsToCSV() {
        ofstream csvFile("benchmark_results.csv");
        if (!csvFile.is_open()) {
            cout << "Error: Could not create CSV file" << endl;
            return;
        }

        csvFile << "Metric,Trie,Hash Table" << endl;
        csvFile << "Build Time (ms)," << trieBuildTime << "," << hashBuildTime << endl;
        csvFile << "Avg Lookup Time (microseconds)," << fixed << setprecision(3)
                << trieAvgLookup << "," << hashAvgLookup << endl;
        csvFile << "Memory Usage (MB)," << (trieMemory / 1024 / 1024) << ","
                << (hashMemory / 1024 / 1024) << endl;
        csvFile << "Words Loaded," << allWords.size() << "," << allWords.size() << endl;

        csvFile.close();
        cout << endl << "Results exported to 'benchmark_results.csv'" << endl;
        cout << "You can open this file in Excel or Google Sheets to create graphs!" << endl;
    }

    void createTextReport() {
        ofstream reportFile("performance_report.txt");
        if (!reportFile.is_open()) {
            cout << "Error: Could not create report file" << endl;
            return;
        }

        reportFile << "============================================================" << endl;
        reportFile << "    TRIE vs HASH TABLE - PERFORMANCE ANALYSIS REPORT" << endl;
        reportFile << "============================================================" << endl;
        reportFile << "Team: Nonchalant" << endl;
        reportFile << "Members: Nicholas Parmigiano, Alen Wu, Sebastien Laguerre" << endl;
        reportFile << "============================================================" << endl << endl;

        reportFile << "DATASET INFORMATION:" << endl;
        reportFile << "------------------------------------------------------------" << endl;
        reportFile << "Total words loaded: " << allWords.size() << endl << endl;

        reportFile << "BUILD PERFORMANCE:" << endl;
        reportFile << "------------------------------------------------------------" << endl;
        reportFile << "Trie build time:       " << trieBuildTime << " ms" << endl;
        reportFile << "Hash Table build time: " << hashBuildTime << " ms" << endl;

        if (trieBuildTime < hashBuildTime) {
            double speedup = (double)hashBuildTime / trieBuildTime;
            reportFile << "Winner: Trie (" << fixed << setprecision(2) << speedup << "x faster)" << endl << endl;
        } else {
            double speedup = (double)trieBuildTime / hashBuildTime;
            reportFile << "Winner: Hash Table (" << fixed << setprecision(2) << speedup << "x faster)" << endl << endl;
        }

        reportFile << "LOOKUP PERFORMANCE (1000 random queries):" << endl;
        reportFile << "------------------------------------------------------------" << endl;
        reportFile << "Trie average:       " << fixed << setprecision(3) << trieAvgLookup << " μs per query" << endl;
        reportFile << "Hash Table average: " << fixed << setprecision(3) << hashAvgLookup << " μs per query" << endl;

        if (trieAvgLookup < hashAvgLookup) {
            double speedup = hashAvgLookup / trieAvgLookup;
            reportFile << "Winner: Trie (" << fixed << setprecision(2) << speedup << "x faster)" << endl << endl;
        } else {
            double speedup = trieAvgLookup / hashAvgLookup;
            reportFile << "Winner: Hash Table (" << fixed << setprecision(2) << speedup << "x faster)" << endl << endl;
        }

        reportFile << "MEMORY USAGE (Approximate):" << endl;
        reportFile << "------------------------------------------------------------" << endl;
        reportFile << "Trie:       ~" << (trieMemory / 1024 / 1024) << " MB" << endl;
        reportFile << "Hash Table: ~" << (hashMemory / 1024 / 1024) << " MB" << endl;

        if (trieMemory < hashMemory) {
            double savings = ((double)(hashMemory - trieMemory) / hashMemory) * 100;
            reportFile << "Winner: Trie (" << fixed << setprecision(0) << savings << "% less memory)" << endl << endl;
        } else {
            double savings = ((double)(trieMemory - hashMemory) / trieMemory) * 100;
            reportFile << "Winner: Hash Table (" << fixed << setprecision(0) << savings << "% less memory)" << endl << endl;
        }

        reportFile << "ANALYSIS & CONCLUSIONS:" << endl;
        reportFile << "------------------------------------------------------------" << endl;
        reportFile << "- Hash Tables typically provide faster average-case lookups (O(1))" << endl;
        reportFile << "- Tries excel at prefix-based queries and autocomplete" << endl;
        reportFile << "- Hash Tables generally use less memory for simple word storage" << endl;
        reportFile << "- Tries provide better worst-case guarantees" << endl << endl;

        reportFile << "RECOMMENDATION:" << endl;
        reportFile << "------------------------------------------------------------" << endl;
        reportFile << "For pure word lookup: Hash Table" << endl;
        reportFile << "For autocomplete/prefix search: Trie" << endl;
        reportFile << "For balanced use case: Consider hybrid approach" << endl;
        reportFile << "============================================================" << endl;

        reportFile.close();
        cout << "Detailed report saved to 'performance_report.txt'" << endl;
    }

public:
    BenchmarkSystem() : trie(nullptr), hashTable(nullptr), trieBuilt(false),
                        hashTableBuilt(false), trieBuildTime(0), hashBuildTime(0),
                        trieAvgLookup(0), hashAvgLookup(0), trieMemory(0), hashMemory(0) {}

    ~BenchmarkSystem() {
        if (trie) delete trie;
        if (hashTable) delete hashTable;
    }

    bool loadWordsFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not open file '" << filename << "'" << endl;
            return false;
        }

        allWords.clear();
        string word;
        while (file >> word) {
            string cleanWord;
            for (char c : word) {
                if (isalpha(c)) {
                    cleanWord += tolower(c);
                }
            }
            if (!cleanWord.empty()) {
                allWords.push_back(cleanWord);
            }
        }
        file.close();

        cout << "Successfully loaded " << allWords.size() << " words from " << filename << endl;
        return !allWords.empty();
    }

    void buildTrie() {
        if (allWords.empty()) {
            cout << "Error: No words loaded. Please load a dataset first." << endl;
            return;
        }

        if (trie) delete trie;
        trie = new Trie();

        cout << "Building Trie with " << allWords.size() << " words..." << endl;

        auto start = high_resolution_clock::now();
        for (const string& word : allWords) {
            trie->insert(word);
        }
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start);

        trieBuildTime = duration.count();
        trieBuilt = true;
        cout << "Trie built successfully in " << trieBuildTime << " ms" << endl;
    }

    void buildHashTable() {
        if (allWords.empty()) {
            cout << "Error: No words loaded. Please load a dataset first." << endl;
            return;
        }

        if (hashTable) delete hashTable;
        hashTable = new HashTable(32768);

        cout << "Building Hash Table with " << allWords.size() << " words..." << endl;

        auto start = high_resolution_clock::now();
        for (const string& word : allWords) {
            hashTable->insert(word);
        }
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start);

        hashBuildTime = duration.count();
        hashTableBuilt = true;
        cout << "Hash Table built successfully in " << hashBuildTime << " ms" << endl;
    }

    void searchWord() {
        if (!trieBuilt && !hashTableBuilt) {
            cout << "Error: Please build at least one data structure first." << endl;
            return;
        }

        string word;
        cout << "Enter word to search: ";
        cin >> word;
        transform(word.begin(), word.end(), word.begin(), ::tolower);

        if (trieBuilt) {
            auto start = high_resolution_clock::now();
            bool found = trie->search(word);
            auto end = high_resolution_clock::now();
            auto duration = duration_cast<nanoseconds>(end - start);

            cout << "Trie: '" << word << "' " << (found ? "FOUND" : "NOT FOUND")
                 << " (Time: " << duration.count() << " ns)" << endl;
        }

        if (hashTableBuilt) {
            auto start = high_resolution_clock::now();
            bool found = hashTable->contains(word);
            auto end = high_resolution_clock::now();
            auto duration = duration_cast<nanoseconds>(end - start);

            cout << "Hash Table: '" << word << "' " << (found ? "FOUND" : "NOT FOUND")
                 << " (Time: " << duration.count() << " ns)" << endl;
        }
    }

    void runBenchmark() {
        if (!trieBuilt || !hashTableBuilt) {
            cout << "Error: Both Trie and Hash Table must be built before benchmarking." << endl;
            return;
        }

        const int NUM_QUERIES = 1000;
        cout << endl << "Running benchmark with " << NUM_QUERIES << " random queries..." << endl;
        cout << string(60, '=') << endl;

        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, allWords.size() - 1);

        vector<string> queryWords;
        for (int i = 0; i < NUM_QUERIES; i++) {
            queryWords.push_back(allWords[dis(gen)]);
        }

        // Benchmark Trie
        auto trieStart = high_resolution_clock::now();
        int trieFound = 0;
        for (const string& word : queryWords) {
            if (trie->search(word)) trieFound++;
        }
        auto trieEnd = high_resolution_clock::now();
        auto trieDuration = duration_cast<microseconds>(trieEnd - trieStart);

        // Benchmark Hash Table
        auto hashStart = high_resolution_clock::now();
        int hashFound = 0;
        for (const string& word : queryWords) {
            if (hashTable->contains(word)) hashFound++;
        }
        auto hashEnd = high_resolution_clock::now();
        auto hashDuration = duration_cast<microseconds>(hashEnd - hashStart);

        // Store results
        trieAvgLookup = trieDuration.count() / (double)NUM_QUERIES;
        hashAvgLookup = hashDuration.count() / (double)NUM_QUERIES;

        // Display results
        cout << endl << "BENCHMARK RESULTS:" << endl;
        cout << string(60, '-') << endl;

        cout << left << setw(30) << "Metric" << setw(15) << "Trie" << setw(15) << "Hash Table" << endl;
        cout << string(60, '-') << endl;

        cout << left << setw(30) << "Total queries:" << setw(15) << NUM_QUERIES << setw(15) << NUM_QUERIES << endl;
        cout << left << setw(30) << "Words found:" << setw(15) << trieFound << setw(15) << hashFound << endl;
        cout << left << setw(30) << "Total time (μs):" << setw(15) << trieDuration.count() << setw(15) << hashDuration.count() << endl;
        cout << left << setw(30) << "Avg time per query (μs):"
             << setw(15) << fixed << setprecision(3) << trieAvgLookup
             << setw(15) << fixed << setprecision(3) << hashAvgLookup << endl;

        cout << string(60, '-') << endl;

        if (trieDuration.count() < hashDuration.count()) {
            double speedup = hashDuration.count() / (double)trieDuration.count();
            cout << "Winner: Trie is " << fixed << setprecision(2) << speedup << "x faster" << endl;
        } else {
            double speedup = trieDuration.count() / (double)hashDuration.count();
            cout << "Winner: Hash Table is " << fixed << setprecision(2) << speedup << "x faster" << endl;
        }

        cout << string(60, '=') << endl;

        // Export results
        exportResultsToCSV();
        createTextReport();
    }

    void displayMemoryUsage() {
        if (!trieBuilt && !hashTableBuilt) {
            cout << "Error: Please build at least one data structure first." << endl;
            return;
        }

        cout << endl << "MEMORY USAGE ESTIMATE:" << endl;
        cout << string(60, '=') << endl;

        if (trieBuilt) {
            trieMemory = estimateTrieMemory();
            cout << "Trie: ~" << (trieMemory / 1024) << " KB (~" << (trieMemory / 1024 / 1024) << " MB)" << endl;
        }

        if (hashTableBuilt) {
            hashMemory = estimateHashTableMemory();
            cout << "Hash Table: ~" << (hashMemory / 1024) << " KB (~" << (hashMemory / 1024 / 1024) << " MB)" << endl;
            cout << "  - Bucket count: " << hashTable->bucketCount() << endl;
            cout << "  - Stored words: " << hashTable->size() << endl;
        }

        cout << string(60, '=') << endl;
        cout << "Note: Memory estimates are approximate" << endl;
    }

    void displayMenu() {
        cout << endl;
        cout << "========================================" << endl;
        cout << "  Word Lookup Performance Benchmark" << endl;
        cout << "========================================" << endl;
        cout << "0. Load Dataset from File" << endl;
        cout << "1. Build Trie" << endl;
        cout << "2. Build Hash Table" << endl;
        cout << "3. Search for a Word" << endl;
        cout << "4. Run Benchmark (Compare Lookup Time)" << endl;
        cout << "5. Display Memory Usage" << endl;
        cout << "6. Exit" << endl;
        cout << "========================================" << endl;
        cout << "Enter your choice: ";
    }

    void run() {
        int choice;
        string filename;

        while (true) {
            displayMenu();
            cin >> choice;

            switch (choice) {
                case 0:
                    cout << "Enter filename (e.g., words.txt): ";
                    cin >> filename;
                    loadWordsFromFile(filename);
                    break;
                case 1:
                    buildTrie();
                    break;
                case 2:
                    buildHashTable();
                    break;
                case 3:
                    searchWord();
                    break;
                case 4:
                    runBenchmark();
                    break;
                case 5:
                    displayMemoryUsage();
                    break;
                case 6:
                    cout << "Exiting program. Goodbye!" << endl;
                    return;
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        }
    }
};

int main() {
    cout << endl << "*** TRIE vs HASH TABLE BENCHMARK SYSTEM ***" << endl;
    cout << "Team: Nonchalant" << endl;
    cout << "Members: Nicholas Parmigiano, Alen Wu, Sebastien Laguerre" << endl;

    BenchmarkSystem system;
    system.run();

    return 0;
}