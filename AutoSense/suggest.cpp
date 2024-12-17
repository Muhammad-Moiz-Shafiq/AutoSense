#include "suggest.h"
#include "Trie.h"
#include "fileHandling.h"

void startProgram(){
    Trie trie;

    // Load the dictionary into the Trie
    string filename = "dictionary.txt"; // Ensure the path is correct
    loadDictionary(trie, filename);

    // Main program loop
    string prefix;
    while (true) {
        cout << "\nEnter a prefix to search (or press 'CTRL' + 'C' to quit): ";
        cin >> prefix;
        for (int i = 0; i < prefix.length(); i++) {
            prefix[i] = tolower(prefix[i]);
        }


        if (prefix == "exit") {
            cout << "Exiting program. Goodbye!\n";
            break;
        }

        // Get suggestions from the Trie
        vector<string> suggestions = trie.autocomplete(prefix);
        if (suggestions.empty()) {
            cout << "No suggestions found for the prefix \"" << prefix << "\".\n";

            // Prompt the user to add the prefix to the dictionary
            char choice;
            cout << "Do you want to add \"" << prefix << "\" to the dictionary? (y/n): ";
            cin >> choice;
            choice = tolower(choice);
            if (choice == 'y') {
                trie.insert(prefix);
                appendWordToFile(prefix, filename);
                cout << "\"" << prefix << "\" has been added to the dictionary.\n";
            } else {
                cout << "Prefix \"" << prefix << "\" was not added.\n";
            }
        } else {
            cout << "Suggestions:\n";
            for (const string& word : suggestions) {
                cout << "- " << word << "\n";
            }
        }
    }
}