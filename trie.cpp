#include "trie.h"
#include <iostream>

Trie::Trie() {
    root = new TrieNode();

}

Trie::~Trie() {
    clear(root);
}

void Trie::clear(TrieNode* node) {
    if (!node) return;
    for (auto& pair : node->children) {
        clear(pair.second);
    }
    delete node;
}

void Trie::insert(const std::string& word) {
    TrieNode* node = root;
    for (char c : word) {
        if (!node->children[c])
            node->children[c] = new TrieNode();
        node = node->children[c];
    }
    node->endOfWord = true;
}

bool Trie::removeHelper(TrieNode* node, const std::string& word, int depth) {
    if (!node) return false;

    if (depth == (int)word.size()) {
        if (!node->endOfWord) return false;
        node->endOfWord = false;
        return node->children.empty();
    }
    char c = word[depth];
    if (!node->children.count(c)) return false;

    bool deletable = removeHelper(node->children[c], word, depth + 1);
    if (deletable) {
        delete node->children[c];
        node->children.erase(c);
        return node->children.empty() && !node->endOfWord;
    }
    return false;
}

bool Trie::search(const std::string& word) {
    TrieNode* node = root;
    for (char c : word) {
        if (!node->children.count(c))
            return false;
        node = node->children[c];
    }
    return node->endOfWord;
}
void Trie::remove(const std::string& word) {
    removeHelper(root,word, 0);
}

bool Trie::startsWith(const std::string &prefix) {
    TrieNode* node = root;
    for (char c : prefix) {
        if (!node->children.count(c))
            return false;
        node = node->children[c];
    }
    return true;
}
