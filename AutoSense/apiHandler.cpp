#include "apiHandler.h"
#include <curl/curl.h>
#include <QJsonDocument>

ApiHandler::ApiHandler(QObject *parent) : QObject(parent) {
    CURLcode result = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (result != CURLE_OK) {
        qDebug() << "Failed to initialize CURL:" << curl_easy_strerror(result);
        // Handle the error appropriately
        emit errorOccurred(QString("Failed to initialize CURL: %1").arg(curl_easy_strerror(result)));
    }
}
ApiHandler::~ApiHandler() {
    curl_global_cleanup();
}
size_t ApiHandler::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
   ((std::string*)userp)->append((char*)contents, size * nmemb);
   return size * nmemb;
}
void ApiHandler::analyzeSentiment(const QString& text) {
   // Prepare JSON data
   QJsonObject json;
   json["text"] = text;
   QJsonDocument doc(json);
   QByteArray data = doc.toJson();
    // Initialize CURL
   CURL *curl;
   CURLcode res;
   std::string response_data;

   curl = curl_easy_init();
    if(curl) {
       struct curl_slist *headers = NULL;
       headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:5000/");
       curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.constData());
       curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
       curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
       curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
           emit errorOccurred(QString("Failed to connect to the analysis server: %1")
                            .arg(curl_easy_strerror(res)));
       } else {
           // Parse JSON response
           QJsonDocument jsonResponse = QJsonDocument::fromJson(
               QString::fromStdString(response_data).toUtf8());
           emit analysisComplete(jsonResponse.object());
       }
        curl_slist_free_all(headers);
       curl_easy_cleanup(curl);
   }else {
    emit errorOccurred("Failed to initialize CURL session");
    return;
   }
   // curl_global_cleanup();
}

void ApiHandler::sendFeedback(const QJsonObject& feedback) {
    QJsonDocument doc(feedback);
    QByteArray data = doc.toJson();

    CURL *curl;
    CURLcode res;
    std::string response_data;

    curl = curl_easy_init();
    if(curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:5000/feedback");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.constData());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            emit errorOccurred(QString("Failed to send feedback: %1")
                             .arg(curl_easy_strerror(res)));
        } else {
            emit feedbackSent(QString::fromStdString(response_data));
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    } else {
        emit errorOccurred("Failed to initialize CURL session for feedback");
    }
} 