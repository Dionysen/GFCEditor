#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Custom
#include "GLDAuxiliaryArea.h"
#include "GLDToolBar.h"
#include "GLDSchemaWidget.h"
#include "GLDEditorWidget.h"
#include "GLDAttributeArea.h"
#include "GLDMenuBar.h"
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
    // 设置菜单栏样式
    void setupMenuBar();
    void setupToolBar();
    void setupStatusBar();
    void setupLayout();
    void setupAuxiliary();
    void setupEditor();

    QWidget* GetMainWindow()
    {
        QWidget* parent = dynamic_cast<QWidget*>(this->parent());
        if (parent)
            return parent;
        else
            return this;
    }

  private:
    // 工具栏widget
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

    // 布局
    QDockWidget* editorDockWidget;
    QDockWidget* attributedockWidget;
    QDockWidget* auxiliarydockWidget;
    QDockWidget* schemaDockWidget;

    QAction* refresh;

  private slots:
    void on_actionAbout_clicked();
    void on_actionHelp_clicked();
};

#endif  // MAINWINDOW_H
