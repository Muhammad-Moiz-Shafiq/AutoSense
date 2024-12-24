#ifndef APIHANDLER_H
#define APIHANDLER_H

#include <QString>
#include <QObject>
#include <QJsonObject>


class ApiHandler : public QObject {
   Q_OBJECT
public:
   explicit ApiHandler(QObject *parent = nullptr);
   ~ApiHandler();
   void analyzeSentiment(const QString& text);
   void sendFeedback(const QJsonObject& feedback);
signals:
   void analysisComplete(const QJsonObject& result);
   void errorOccurred(const QString& error);
   void feedbackSent(const QString& response);
private:
   static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
   QString response_string;
};
#endif // APIHANDLER_H