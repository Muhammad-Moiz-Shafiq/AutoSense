#include "mainWindow.h"
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
        "    padding: 5px;margin: 5px 30px;"
        "    background-color: rgb(58, 59, 61);font-size: 16px;"
        "}"
        "QPushButton {"
        "    background-color:rgb(58, 59, 61);"
        "    color: white;"
        "    border: none;margin: 5px 30px;font-size: 18px;"
        "    padding: 8px 15px;"
        "    border-radius: 8px;"
        "}"
        "QPushButton:hover {"
        "    background-color:rgb(92, 127, 163);font-style:italic;"
        "}"
        "QListWidget {"
        "    border: 1px solid #ccc;margin: 5px 30px;padding:5px;"
        "    background-color:rgb(58, 59, 61);font-size: 16px;"
        "    border-radius: 8px;"
        "}"
        "#logoLabel {margin: 50px 20px 50px;"
        "font-size: 40px;text-align:center;"
        " font-weight: bold;"
        " color: rgb(162, 230, 252);}"
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
