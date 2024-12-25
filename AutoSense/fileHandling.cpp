#include "Trie.h"
#include "fileHandling.h"

void loadDictionary(Trie& trie, const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Unable to open " << filename << endl;
        return;
    }

    string word;
    while (file >> word) { // Read word by word
        trie.insert(word);
    }
    file.close();
    //cout << "Dictionary loaded successfully.\n";
}
// Function to append a word to the dictionary file
void appendWordToFile(const string& word, const string& filename) {
    ofstream file(filename, ios::app); // Open file in append mode
    if (!file) {
        cerr << "Error: Unable to open " << filename << " for writing.\n";
        return;
    }
    file <<"\n" <<word ; // Write the word to the file
    file.close();
}