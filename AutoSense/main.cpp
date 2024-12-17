#include <QApplication>
#include "mainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.setWindowTitle("Trie Autocompletion");
    window.resize(500, 600);
    window.show();
    return app.exec();
}