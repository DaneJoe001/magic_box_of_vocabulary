#include <QApplication>
#include <QTimer>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow mainWindow;

    return a.exec();
}
