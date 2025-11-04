#ifndef TRIE_H
#define TRIE_H

#include <unordered_map>
#include <string>

struct TrieNode {
    bool endOfWord;
    std::unordered_map<char, TrieNode *> children;

    TrieNode() : endOfWord(false) {}
};


class Trie {
private:
    TrieNode* root;

    bool removeHelper(TrieNode* node, const std::string& word, int depth);
    void clear(TrieNode* node);

public:
    Trie();
    ~Trie();

    void insert(const std::string& word);
    void remove(const std::string& word);
    bool search(const std::string& word);
    bool startsWith(const std::string& prefix);
};

#endif
