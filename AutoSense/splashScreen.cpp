#include "splashScreen.h"
#include <QPainter>
#include <QTimer>

SplashScreen::SplashScreen() {
    QPixmap pixmap(500, 300);
    QLinearGradient gradient(0, 0, 500, 300);
      gradient.setColorAt(0, QColor(37, 33, 33)); // Dark grey
    gradient.setColorAt(1, QColor(58, 59, 61)); // Slightly lighter grey
    QPainter painter(&pixmap);
    painter.fillRect(pixmap.rect(), gradient);

    painter.setPen(QColor(0, 0, 0, 160)); // Semi-transparent black for shadow
    painter.setFont(QFont("Times New Roman", 24, QFont::Bold,true));
    painter.drawText(pixmap.rect().adjusted(2, 2, 2, 2), Qt::AlignCenter, "Analyzing...");
    painter.setPen(QColor(162, 230, 252)); // Light blue for text
    painter.drawText(pixmap.rect(), Qt::AlignCenter, "Analyzing...");

    setPixmap(pixmap);
}

void SplashScreen::showSplash() {
    show();
    QTimer::singleShot(3000, this, &QSplashScreen::close);
}