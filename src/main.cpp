
#define CUSTOM_WINDOW
#define ENABLE_TEST



#ifdef CUSTOM_WINDOW
#include "CustomWindow.h"
#endif

#include <QApplication>
#include "MainWindow.h"
#include <windows.h>
#include <gtest/gtest.h>
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

#ifdef ENABLE_TEST
    ::testing::InitGoogleTest(&argc, argv);
    int res = RUN_ALL_TESTS();
#endif

    return a.exec();
}
