#include <QApplication>
#include <QTimer>

#include "mainwindow.h"
#include "databaseoperation.h"
#include "wordcard.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    WordCard test;
    //test.testDataInit();
    test.show();

    //DatabaseOperation db;
    return a.exec();
}
