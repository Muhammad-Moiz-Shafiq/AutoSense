#include <QApplication>
#include <QLabel>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Main window
    QWidget window;
    window.setWindowTitle("Styled Qt GUI");
    window.resize(500, 400);

    // Set style sheet for the main window
    window.setStyleSheet("background-color: #2E3440; color: #ECEFF4;");

    // Label
    QLabel *label = new QLabel("Welcome to My Styled Qt App!");
    label->setAlignment(Qt::AlignCenter);

// Button
    QPushButton *button = new QPushButton("Click Me!");
    button->setStyleSheet(
        "QPushButton {"
        "   background-color: #5E81AC;"
        "   color: white;"
        "   font-size: 16px;"
        "   padding: 8px;"
        "   border: 2px solid #88C0D0;"
        "   border-radius: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #81A1C1;"
        "}"
    );

    // Set style sheet for label
    label->setStyleSheet(
        "QLabel {"
        "   font-size: 20px;"
        "   font-weight: bold;"
        "   color: #88C0D0;"
        "   background-color: #3B4252;"
        "   border: 2px solid #4C566A;"
        "   border-radius: 10px;"
        "   padding: 10px;"
        "}"
    );

    // Layout
    QVBoxLayout *layout = new QVBoxLayout(&window);
    layout->addWidget(label);
    layout->addWidget(button);

    // Show window
    window.setLayout(layout);
    window.show();

    return app.exec();
}
