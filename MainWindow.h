#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// 自定义控件头文件
#include "GLDAuxiliaryArea.h"
#include "GLDToolBar.h"
#include "GLDSchemaWidget.h"
#include "GLDEditorWidget.h"
#include "GLDAttributeArea.h"
// Qt头文件
#include "qmainwindow.h"
#include <QMainWindow>
#include <QSplitter>
namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

  private:
    // 设置菜单栏样式
    void setMenuBar();

    // 链接模块信号
    void connectToolBar();
    void connectAux();

  private:
    // 工具栏widget
    GLDToolBar* p_toolBar;

    // schema widget，左右树控件
    GLDSchemaWidget* p_schemaWiddget;

    // 文本编辑区域
    GLDEditorWidget* p_editorWidget;

    GLDAuxiliaryArea* p_auxiliaryArea;
    GLDAttributeArea* p_attributeArea;

    // 布局
    QDockWidget* attributedockWidget;
    QDockWidget* auxiliarydockWidget;
    QDockWidget* schemaDockWidget;

    Ui::MainWindow* ui;

  private slots:
    void on_actionAbout_clicked();
    void on_actionHelp_clicked();
};

#endif  // MAINWINDOW_H
