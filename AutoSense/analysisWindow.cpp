#include "analysisWindow.h"
#include "apiHandler.h"
#include <QJsonObject>
#include <QJsonValue>
AnalysisWindow::AnalysisWindow(const QString& textToAnalyze, QWidget *parent)
   : QMainWindow(parent)
{
   setupUI();
   performAnalysis(textToAnalyze);
}

void AnalysisWindow::setupUI() {
   // Set window properties
   setWindowTitle("Analysis Results");
   resize(600, 600);
    // Create central widget and layout
   centralWidget = new QWidget(this);
   layout = new QVBoxLayout(centralWidget);
   
   // Create and style the result label
   resultLabel = new QLabel("Analyzing...", this);
   resultLabel->setAlignment(Qt::AlignCenter);
   
   // Add widgets to layout
   layout->addWidget(resultLabel);
   
   // Set central widget
   setCentralWidget(centralWidget);
    // Apply styling
   this->setStyleSheet(
       "QMainWindow {"
       "    background-color:rgb(37, 33, 33);"
       "}"
       "QLabel {"
       "    color: rgb(162, 230, 252);"
       "    font-size: 18px;"
       "    margin: 10px;"
       "    padding: 10px;"
       "}"
   );
}

void AnalysisWindow::performAnalysis(const QString& text) {
   apiHandler = new ApiHandler(this);
   
   // Connect signals
   connect(apiHandler, &ApiHandler::analysisComplete, 
           this, &AnalysisWindow::handleAnalysisResult);
   connect(apiHandler, &ApiHandler::errorOccurred, 
           this, &AnalysisWindow::handleError);
   
   // Start analysis
   apiHandler->analyzeSentiment(text);
}

void AnalysisWindow::handleAnalysisResult(const QJsonObject& result) {
   QString sentiment = result["predicted_sentiment"].toString();
   QJsonObject scores = result["sentiment_scores"].toObject();
   
   // Format the results
   QString resultText = QString(
       "<h2>Analysis Results</h2>"
       "<p>Text Sentiment: <b>%1</b></p>"
       "<h3>Sentiment Scores:</h3>"
       "<p>Positive: %2</p>"
       "<p>Negative: %3</p>"
       "<p>Neutral: %4</p>"
   ).arg(sentiment.toUpper())
    .arg(scores["positive"].toDouble(), 0, 'f', 4)
    .arg(scores["negative"].toDouble(), 0, 'f', 4)
    .arg(scores["neutral"].toDouble(), 0, 'f', 4);
    resultLabel->setText(resultText);
}
void AnalysisWindow::handleError(const QString& error) {
   resultLabel->setText("Error: " + error);
}
