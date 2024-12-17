#include "trie.h"

// Initialize the Trie constructor
Trie::Trie() {
    root = new TrieNode(); // Initialize the root node
}

// Destructor to clean up memory
Trie::~Trie() {
    deleteTrie(root);
}

// Insert a word into the Trie
void Trie::insert(const string& word) {
    TrieNode* current = root;
    for (char ch : word) {
        if (current->children.find(ch) == current->children.end()) {
            current->children[ch] = new TrieNode(); // Create a new node if it doesn't exist
        }
        current = current->children[ch]; // Move to the next node
    }
    current->isEndOfWord = true; // Mark the end of the word
}

// Search for a word in the Trie
bool Trie::search(const string& word) {
    TrieNode* current = root;
    for (char ch : word) {
        if (current->children.find(ch) == current->children.end()) {
            return false; // If a character is missing, word doesn't exist
        }
        current = current->children[ch]; // Move to the next node
    }
    return current->isEndOfWord; // Check if it's a valid word
}

// Check if there is any word that starts with a given prefix
bool Trie::startsWith(const string& prefix) {
    TrieNode* current = root;
    for (char ch : prefix) {
        if (current->children.find(ch) == current->children.end()) {
            return false; // If a character is missing, prefix doesn't exist
        }
        current = current->children[ch]; // Move to the next node
    }
    return true; // Prefix exists
}

// Helper function to delete the Trie
void Trie::deleteTrie(TrieNode* node) {
    for (auto& pair : node->children) {
        deleteTrie(pair.second);
    }
    delete node;
}

// Get all words in the Trie that start with a given prefix
void Trie::getSuggestions(TrieNode* node, string currentWord, vector<string>& suggestions) {
    if (node->isEndOfWord) {
        suggestions.push_back(currentWord); // If it's a word, add to suggestions
    }

    for (auto& [ch, childNode] : node->children) {
        getSuggestions(childNode, currentWord + ch, suggestions); // Recursively get all words
    }
}

vector<string> Trie::autocomplete(const string& prefix) {
    TrieNode* current = root;
    vector<string> suggestions;

    // Traverse the Trie to the end of the prefix
    for (char ch : prefix) {
        if (current->children.find(ch) == current->children.end()) {
            return suggestions; // No suggestions if prefix doesn't exist
        }
        current = current->children[ch];
    }

    // Get all words starting with the prefix
    getSuggestions(current, prefix, suggestions);
    return suggestions;
}

