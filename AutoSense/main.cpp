#include <QApplication>
#include "mainWindow.h"
#include "splashScreen.h"
#include <QTimer>


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon("D:/Uni/Sem 3/DSA/Project/AutoSense/assets/icon.png"));
    // splash screen
   // Create and show the splash screen
    SplashScreen splash;
    splash.showSplash();

    //Main Window
    MainWindow window;
    window.setWindowTitle("AutoSense");
    window.resize(600, 600);
    QTimer::singleShot(3000, &window, &MainWindow::show); // Show main window after 3 seconds

    return app.exec();
}