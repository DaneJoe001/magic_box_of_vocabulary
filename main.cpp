#include <QApplication>
#include <QTimer>

#include "window_main.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    WindowMain mainWindow;

    return a.exec();
}
