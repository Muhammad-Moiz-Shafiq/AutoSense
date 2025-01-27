#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include "trie.h"
#include "fileHandling.h"
#include "analysisWindow.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onTextChanged(const QString &text);
    void onSuggestionClicked(QListWidgetItem *item);
    void onAnalyzeClicked();
    void onAnalysisWindowClosed();

private:
    QLineEdit *inputField;
    QListWidget *suggestionBox;
    QPushButton *analyzeButton;
    QLabel *logoLabel;
    Trie trie;
};

#endif // MAINWINDOW_H
