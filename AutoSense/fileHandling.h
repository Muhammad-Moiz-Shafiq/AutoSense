#ifndef FILEHANDLING_H
#define FILEHANDLING_H

#include <string>
#include "Trie.h"
#include <fstream>
#include <iostream>

// Function to load dictionary into Trie
void loadDictionary(Trie& trie, const string& filename);

// Function to append a word to the dictionary file
void appendWordToFile(const string& word, const string& filename);

#endif
