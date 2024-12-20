#ifndef ANALYSISWINDOW_H
#define ANALYSISWINDOW_H
#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include "apiHandler.h"
class AnalysisWindow : public QMainWindow {
   Q_OBJECT
public:
   explicit AnalysisWindow(const QString& textToAnalyze, QWidget *parent = nullptr);
private slots:
   void handleAnalysisResult(const QJsonObject& result);
   void handleError(const QString& error);
private:
   void setupUI();
   void performAnalysis(const QString& text);
   
   QLabel* resultLabel;
   QWidget* centralWidget;
   QVBoxLayout* layout;
   ApiHandler* apiHandler;
};
#endif // ANALYSISWINDOW_H