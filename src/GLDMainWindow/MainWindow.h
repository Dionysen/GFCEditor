#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma once
// Custom
#include "GLDAuxiliaryArea.h"
#include "GLDToolBar.h"
#include "GLDSchemaWidget.h"
#include "GLDEditorWidget.h"
#include "GLDAttributeArea.h"
#include "GLDMenuBar.h"
#include "CustomWindow.h"
// Qt
#include <QDockWidget>
#include <QMainWindow>
#include <QSplitter>
#include <QAction>

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

  private:
    // 初始化各个控件
    void setupMenuBar();
    void setupToolBar();
    void setupStatusBar();
    void setupDockingLayout();
    void setupNoDockingLayout();
    void setupAuxiliary();
    void setupEditor();

    // 使用自定义窗口，返回自定义窗口，否则返回this
    QWidget*      GetMainWindow();
    CustomWindow* GetCustomWindow();

    inline GLDToolBar* GetToolBar()
    {
        return p_toolBar;
    }


  private:
    // 工具栏、菜单栏、状态栏
    GLDToolBar* p_toolBar;
    GLDMenuBar* p_menuBar;
    QStatusBar* p_StatusBar;

    // schema widget，左右树控件
    GLDSchemaWidget* p_schemaWiddget;

    // 文本编辑区域
    GLDEditorWidget* p_editorWidget;

    // 辅助区域
    GLDAuxiliaryArea* p_auxiliaryArea;

    // 属性区域
    GLDAttributeArea* p_attributeArea;

    QAction* refresh;

  private slots:
    void on_actionAbout_clicked();
    void on_actionHelp_clicked();
};

#endif  // MAINWINDOW_H
