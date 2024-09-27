
// #define CUSTOM_WINDOW

#ifdef CUSTOM_WINDOW
#include "CustomWindow.h"
#endif

#include <QApplication>
#include "MainWindow.h"
#include <windows.h>

int main(int argc, char* argv[])
{

#ifdef _DEBUG
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
#endif

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
