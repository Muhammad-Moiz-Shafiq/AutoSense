#include "analysisWindow.h"
#include "apiHandler.h"
#include <QJsonObject>
#include <QJsonValue>
#include <QInputDialog>
#include <QMessageBox>
#include <QJsonDocument>

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
   inText = new QString();
   
   // Create and style the result label
   resultLabel = new QLabel("Analyzing...", this);
   resultLabel->setAlignment(Qt::AlignCenter);
   
   // Create input field for feedback
   feedbackInput = new QLineEdit(this);
   feedbackInput->setPlaceholderText("Is the analysis correct? (yes/no)");
   
   // Create submit button
   submitButton = new QPushButton("Submit Feedback", this);
   connect(submitButton, &QPushButton::clicked, this, &AnalysisWindow::submitFeedback);
   
   // Add widgets to layout
   layout->addWidget(resultLabel);
   layout->addWidget(feedbackInput);
   layout->addWidget(submitButton);
   
   // Set central widget
   setCentralWidget(centralWidget);
   
   // Apply styling
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
   connect(apiHandler, &ApiHandler::feedbackSent, 
           this, &AnalysisWindow::handleFeedbackResponse);  // Connect feedbackSent signal
    inText = new QString(text);
   // Start analysis
   apiHandler->analyzeSentiment(text);
}

void AnalysisWindow::handleAnalysisResult(const QJsonObject& result) {
   QString sentiment = result["predicted_sentiment"].toString();
   QJsonObject scores = result["sentiment_scores"].toObject();
   if (sentiment == "positive") {
        sentiment = "POSITIVE 😊";
       resultLabel->setStyleSheet("color: green;");
   } else if (sentiment == "negative") {
         sentiment = "NEGATIVE ☹️";
       resultLabel->setStyleSheet("color: red;");
   } else {
            sentiment = "NEUTRAL 😐";
       resultLabel->setStyleSheet("color: yellow;");
   }
   // Format the results
   QString resultText = QString(
       "<h2>Analysis Results</h2>"
       "<p>Text connotation: <b>%1</b></p>"
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

void AnalysisWindow::submitFeedback() {
    QString feedback = feedbackInput->text().trimmed();
    
    if (feedback.isEmpty() || (feedback.toLower() != "yes" && feedback.toLower() != "no")) {
        QMessageBox::warning(this, "Input Error", "Please provide correct feedback.");
        return;
    }

    // Check if feedback is correct
    if (feedback.toLower() == "yes") {
        QMessageBox::information(this, "Thank You", "Thank you for your feedback!");
        emit analysisWindowClosed();  // Emit the custom signal
        this->close();  // Close the analysis window and return to main window
    } else {
        // Prompt for correct sentiment
        QStringList sentiments = {"positive", "negative", "neutral"};
        bool ok;
        QString correctSentiment = QInputDialog::getItem(this, "Select Correct Sentiment",
                                                         "What is the correct sentiment?",
                                                         sentiments, 0, false, &ok);
        if (ok && !correctSentiment.isEmpty()) {
            QJsonObject feedbackJson;
            feedbackJson["text"] = *inText;  // Use the original text
            feedbackJson["correct_sentiment"] = correctSentiment;
            apiHandler->sendFeedback(feedbackJson);
        }
    }
}

void AnalysisWindow::handleError(const QString& error) {
   resultLabel->setText("Error: " + error);
}

void AnalysisWindow::handleFeedbackResponse(const QString& response) {
    QMessageBox::information(this, "Feedback Sent", "Your feedback has been recorded.");
   // std::cout << "Emitting analysisWindowClosed signal" << std::endl;  // Debugging statement
    emit analysisWindowClosed();  // Emit the custom signal
    this->close();  // Close the analysis window after feedback is sent
}
