#include "mainWindow.h"
#include "analysisWindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), inputField(new QLineEdit(this)),
      suggestionBox(new QListWidget(this)), analyzeButton(new QPushButton("Analyze", this)) {
//styling
    this->setStyleSheet(
        "QMainWindow {"
        "    background-color:rgb(37, 33, 33);"
        "}"
        "QLineEdit {"
        "    border: 1px solid #ccc;"
        "    border-radius: 8px;"
        "    padding: 10px;"
        "    margin: 10px 30px;"
        "    background-color: rgb(58, 59, 61);"
        "    font-size: 18px;"
        "    color: white;"
        "}"
        "QPushButton {"
        "    background-color:rgb(58, 59, 61);"
        "    color: white;"
        "    border: none;"
        "    margin: 10px 30px;"
        "    font-size: 18px;"
        "    padding: 10px 20px;"
        "    border-radius: 8px;"
        "}"
        "QPushButton:hover {"
        "    background-color:rgb(92, 127, 163);"
        "    font-style:italic;"
        "}"
        "QListWidget {"
        "    border: 1px solid #ccc;"
        "    margin: 10px 30px;"
        "    padding:10px;"
        "    background-color:rgb(58, 59, 61);"
        "    font-size: 16px;"
        "    border-radius: 8px;"
        "    color: white;"
        "}"
        "#logoLabel {"
        "    margin: 50px 20px 50px;"
        "    font-size: 40px;"
        "    text-align:center;"
        "    font-weight: bold;"
        "    color: rgb(162, 230, 252);"
        "}"
    );

     QLabel *logoLabel = new QLabel("A U T O S E N S E", this);
    logoLabel->setAlignment(Qt::AlignCenter);
    logoLabel->setObjectName("logoLabel");
    inputField->setPlaceholderText("Enter text here");
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->addWidget(logoLabel, 0, Qt::AlignCenter);
    layout->addWidget(inputField);
    layout->addWidget(suggestionBox);
    layout->addWidget(analyzeButton);
    setCentralWidget(centralWidget);

    connect(inputField, &QLineEdit::textChanged, this, &MainWindow::onTextChanged);
    connect(suggestionBox, &QListWidget::itemClicked, this, &MainWindow::onSuggestionClicked);
    connect(analyzeButton, &QPushButton::clicked, this, &MainWindow::onAnalyzeClicked);

    loadDictionary(trie, "D:/Uni/Sem 3/DSA/Project/AutoSense/assets/dictionary.txt");
}

void MainWindow::onTextChanged(const QString &text) {
    suggestionBox->clear();
    if (text.isEmpty()) return;

    // Split the text by spaces to get the current word
    QStringList words = text.split(' ', Qt::SkipEmptyParts);
    QString currentWord = words.isEmpty() ? "" : words.last();

    // Display suggestions for the current word while typing
    std::vector<std::string> suggestions = trie.autocomplete(currentWord.toStdString());
    if (!suggestions.empty()) {
        // Display suggestions
        for (const auto &word : suggestions) {
            suggestionBox->addItem(QString::fromStdString(word));
        }
    }

    // Check if the last character is a space
    if (text.endsWith(' ')) {
        // If the last word has no suggestions, prompt to add it to the dictionary
        if (words.size() >= 1) { // Ensure there is a previous word
            QString previousWord = words[words.size() - 1]; // Get the previous word
            
            // Remove trailing punctuation
            previousWord = previousWord.trimmed();
            if (previousWord.endsWith(QChar::fromLatin1('.')) || previousWord.endsWith(QChar::fromLatin1(',')) ||
                previousWord.endsWith(QChar::fromLatin1('!')) || previousWord.endsWith(QChar::fromLatin1('?')) || previousWord.endsWith(QChar::fromLatin1(';')) || previousWord.endsWith(QChar::fromLatin1(':'))) {
                previousWord.chop(1); // Remove the last character (punctuation)
            }

            std::vector<std::string> previousSuggestions = trie.autocomplete(previousWord.toStdString());
            if (previousSuggestions.empty() && !previousWord.isEmpty()) {
                // Prompt to add word if no suggestions found
                QMessageBox::StandardButton reply = QMessageBox::question(this, 
                    "Add Word", "No suggestions found for '" + previousWord + "'. Do you want to add it to the dictionary?", 
                    QMessageBox::Yes | QMessageBox::No);

                if (reply == QMessageBox::Yes) {
                    trie.insert(previousWord.toStdString());
                    appendWordToFile(previousWord.toStdString(), "D:/Uni/Sem 3/DSA/Project/AutoSense/assets/dictionary.txt");
                    QMessageBox::information(this, "Word Added", "Word added successfully!");
                }
            }
        }
    }
}

void MainWindow::onSuggestionClicked(QListWidgetItem *item) {
    QString currentText = inputField->text();
    QStringList words = currentText.split(' ', Qt::SkipEmptyParts);

    if (!words.isEmpty()) {
        // Replace the last word with the selected suggestion
        words.last() = item->text();
    }

    // Join the words back into a single string
    inputField->setText(words.join(' '));
}

void MainWindow::onAnalyzeClicked() {
    QString text = inputField->text();
    AnalysisWindow* analysisWindow = new AnalysisWindow(text);
    
    // Connect the custom signal to the slot
    connect(analysisWindow, &AnalysisWindow::analysisWindowClosed, this, &MainWindow::onAnalysisWindowClosed);
    
    //cout << "Opening AnalysisWindow" << endl;  // Debugging statement
    analysisWindow->show();
    this->hide();  // Hide the main window
}

void MainWindow::onAnalysisWindowClosed() {
    //cout << "Analysis window closed" << endl;  // Debugging statement
    this->show();  // Show the main window again when AnalysisWindow is closed
}
