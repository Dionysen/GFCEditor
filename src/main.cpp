#include <QApplication>
#include "MainWindow.h"
#include "CustomWindow.h"

// #define CUSTOM_WINDOW

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
#ifdef CUSTOM_WINDOW
    CustomWindow w;
    MainWindow   mainWindow(&w);
    w.setCentent(&mainWindow);
    w.show();
#else
    MainWindow w;
    w.show();
#endif
    return a.exec();
}
