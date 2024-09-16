#include <QApplication>
#include "MainWindow.h"

#include <qDebug>
#include <QTextStream>
#include "CustomWindow.h"
int main(int argc, char* argv[])
{

#if 1
    QApplication a(argc, argv);

    CustomWindow w;

    MainWindow mainWindow(&w);
    w.setCentent(&mainWindow);
    w.show();
    return a.exec();
#endif

#if 0
    std::cout << "DS";
    ExpressParser parser;
    parser.parse("resources/GFC3X1.exp");  // 请替换为你的EXPRESS文件路径
    parser.printEntities();
#endif
    return 0;
}
