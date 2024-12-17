#ifndef TRIE_H
#define TRIE_H

#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

// Define the Trie Node structure
class TrieNode{
public:
    unordered_map<char, TrieNode*> children; // Maps characters to child nodes
    bool isEndOfWord; // Marks if the current node is the end of a word

    TrieNode(){
        isEndOfWord = false;
    }
};
class Trie{
private:
    TrieNode* root;
public:
    Trie();
    ~Trie();

    // Insert a word into the Trie
    void insert(const string& word);

    // Search for a word in the Trie
    bool search(const string& word);

    // Check if there is any word that starts with a given prefix
    bool startsWith(const string& prefix);

    void getSuggestions(TrieNode* node, string currentWord, vector<string>& suggestions);

    vector<string> autocomplete(const string& prefix);

    //helper function to delete Trie
    void deleteTrie(TrieNode* node);
};

#endif