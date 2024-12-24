#ifndef ANALYSISWINDOW_H
#define ANALYSISWINDOW_H
#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <iostream>
#include "apiHandler.h"
class AnalysisWindow : public QMainWindow {
   Q_OBJECT
public:
   explicit AnalysisWindow(const QString& textToAnalyze, QWidget *parent = nullptr);
signals:
   void analysisWindowClosed();  // Custom signal for window closure
private slots:
   void handleAnalysisResult(const QJsonObject& result);
   void handleError(const QString& error);
   void handleFeedbackResponse(const QString& response);
   void submitFeedback();
private:
   void setupUI();
   void performAnalysis(const QString& text);
   
   QLabel* resultLabel;
   QWidget* centralWidget;
   QVBoxLayout* layout;
   ApiHandler* apiHandler;
   QLineEdit* feedbackInput;
   QPushButton* submitButton;
};
#endif // ANALYSISWINDOW_H