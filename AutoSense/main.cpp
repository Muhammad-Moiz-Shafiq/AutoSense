#include <QApplication>
#include "mainWindow.h"
#include <QStyleFactory>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon("D:/Uni/Sem 3/DSA/Project/AutoSense/assets/icon.png"));
    //app.setStyle(QStyleFactory::create("Fusion")); //built-in plataform independent style
    MainWindow window;
    window.setWindowTitle("Trie Autocompletion");
    window.resize(500, 600);
    window.show();
    return app.exec();
}