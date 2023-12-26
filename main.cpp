#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setStyleSheet("QMainWindow { background: #FFFFFF; }"
                      "QTableView { selection-background-color: #3399ff; }");
    MainWindow mainWindow;
    mainWindow.show();
    return app.exec();
}
