#include "analysisWindow.h"
#include "apiHandler.h"
#include <QJsonObject>
#include <QJsonValue>
#include <QInputDialog>
#include <QMessageBox>
#include <QJsonDocument>
#include <cstdlib>

AnalysisWindow::AnalysisWindow(const QString& textToAnalyze, QWidget *parent)
   : QMainWindow(parent)
{
   setupUI();
   performAnalysis(textToAnalyze);
}

void AnalysisWindow::setupUI() {
   // Set window properties
   setWindowTitle("Analysis Results");
   resize(700, 700);
   
   // Create central widget and layout
   centralWidget = new QWidget(this);
   layout = new QVBoxLayout(centralWidget);
   
   // Add project name label
   QLabel *logoLabel = new QLabel("A U T O S E N S E", this);
   logoLabel->setAlignment(Qt::AlignCenter);
   logoLabel->setStyleSheet("font-size: 40px; font-weight: bold; color: rgb(162, 230, 252);");
   layout->addWidget(logoLabel); // Add project name at the top

   // Create and style the result label
   resultLabel = new QLabel("Analyzing...", this);
   resultLabel->setAlignment(Qt::AlignCenter);


   // Create and style the tip label
   tipLabel = new QLabel("Tip is based on result.", this);
   tipLabel->setAlignment(Qt::AlignCenter);

   // Create input field for feedback
   feedbackInput = new QLineEdit(this);
   feedbackInput->setPlaceholderText("Is the analysis correct? (yes/no)");

   // Create submit button
   submitButton = new QPushButton("Submit Feedback", this);
   connect(submitButton, &QPushButton::clicked, this, &AnalysisWindow::submitFeedback);
   
   // Add widgets to layout
   layout->addWidget(resultLabel); // Centered result label
   layout->addWidget(tipLabel);    // Tip label below result label
   layout->addWidget(feedbackInput); // Feedback input field
   layout->addWidget(submitButton); // Submit button
   
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
       "#resultLabel {"
        "   font-size: 24px;"
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

    // tips for each sentiment type
   QStringList positiveTips = {
       "Great job! Keep up the positive work!",
       "When words spread joy, the world becomes a better place.",
       "Every positive word is a seed for a brighter tomorrow.",
       "Good vibes only! Cherish the positivity you bring.",
       "A spark of positivity can light up a thousand hearts. Keep shining!"
   };
   QStringList negativeTips = {
       "Don't be discouraged, every setback is a setup for a comeback.",
       "Consider revising your approach for better outcomes.",
       "Think about the impact of your words.",
       "It's okay to have challenges; learn from them!",
       "Words may sting, but they also guide us to grow stronger."
   };
   QStringList neutralTips = {
    "Every neutral moment holds the potential for greatness.",
       "Try to add more emotion to your text.",
       "Even in neutrality lies the power to create something extraordinary.",
       "Think about the impact of your words.",
       "Neutral thoughts pave the way for clarity and purpose."
   };

   // Randomly select a tip based on sentiment
   QString selectedTip;

   if (sentiment == "positive") {
        sentiment = "POSITIVE 😊";
        resultLabel->setStyleSheet("color: green;font-size: 20px; font-weight: bold;");
       tipLabel->setText(positiveTips.at(std::rand() % positiveTips.size()));
       //tipLabel->setText(positiveTips.at(qrand() % positiveTips.size()));
       tipLabel->setStyleSheet("color: green;font-size: 18px; font-style: italic;");
   } else if (sentiment == "negative") {
         sentiment = "NEGATIVE ☹️";
         resultLabel->setStyleSheet("color: red;font-size: 20px; font-weight: bold;");
       tipLabel->setText(negativeTips.at(std::rand() % negativeTips.size()));
      // tipLabel->setText(negativeTips.at(qrand() % negativeTips.size()));
         tipLabel->setStyleSheet("color: red;font-size: 18px; font-style: italic;");
   } else {
            sentiment = "NEUTRAL 😐";
            resultLabel->setStyleSheet("color: rgb(162, 230, 252);font-size: 20px; font-weight: bold;");
         tipLabel->setText(neutralTips.at(std::rand() % neutralTips.size()));
        // tipLabel->setText(neutralTips.at(qrand() % neutralTips.size()));
            tipLabel->setStyleSheet("color: rgb(162, 230, 252);font-size: 18px; font-style: italic;");
   }
   // Format the results
   QString resultText = QString(
       "<h1>Analysis Results</h1>"
       "<h2>Text connotation: <b>%1</b></h2>"
   ).arg(sentiment.toUpper());
    
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
    this->close();  // Closing the analysis window after feedback is sent
}
