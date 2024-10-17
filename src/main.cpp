
#define CUSTOM_WINDOW  // 是否启用自定义窗口
// #define ENABLE_TEST    // 是否启用单元测试

#ifdef CUSTOM_WINDOW
#include "CustomWindow.h"
#endif

#include <QApplication>
#include "MainWindow.h"
#include <windows.h>
#include <gtest/gtest.h>
int main(int argc, char* argv[])
{

/* Debug模式下启用调试输出 */
#ifdef _DEBUG
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
#endif

    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(true);

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
