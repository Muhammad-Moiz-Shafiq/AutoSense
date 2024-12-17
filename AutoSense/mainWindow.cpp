#include "mainWindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), inputField(new QLineEdit(this)),
      suggestionBox(new QListWidget(this)), analyzeButton(new QPushButton("Analyze", this)) {
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->addWidget(inputField);
    layout->addWidget(suggestionBox);
    layout->addWidget(analyzeButton);
    setCentralWidget(centralWidget);

    connect(inputField, &QLineEdit::textChanged, this, &MainWindow::onTextChanged);
    connect(suggestionBox, &QListWidget::itemClicked, this, &MainWindow::onSuggestionClicked);
    connect(analyzeButton, &QPushButton::clicked, this, &MainWindow::onAnalyzeClicked);

    loadDictionary(trie, "D:/Uni/Sem 3/DSA/Project/AutoSense/dictionary.txt");
}

void MainWindow::onTextChanged(const QString &text) {
    suggestionBox->clear();
        if (text.isEmpty()) return;

        std::vector<std::string> suggestions = trie.autocomplete(text.toStdString());
        if (suggestions.empty()) {
            // Prompt to add word if no suggestions found
            QMessageBox::StandardButton reply = QMessageBox::question(this, 
                "Add Word", "No suggestions found. Add to dictionary?", 
                QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::Yes) {
                trie.insert(text.toStdString());
                appendWordToFile(text.toStdString(), "dictionary.txt");
                QMessageBox::information(this, "Word Added", "Word added successfully!");
            }
        } else {
            // Display suggestions
            for (const auto &word : suggestions) {
                suggestionBox->addItem(QString::fromStdString(word));
            }
        }
}

void MainWindow::onSuggestionClicked(QListWidgetItem *item) {
    inputField->setText(item->text());
}

void MainWindow::onAnalyzeClicked() {
   // Open a new window or page for further functionality
        QMessageBox::information(this, "Analyze", "Analyze button clicked!");
        // Replace this with your new window logic
}
