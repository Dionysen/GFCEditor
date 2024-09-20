#include "GLDAuxiliaryArea.h"
#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QLabel>
#include "GLDSearchWindow.h"
#include "GLDErrorWindow.h"

GLDAuxiliaryArea::GLDAuxiliaryArea(const QString& title, QWidget* parent)
    : QDockWidget(title, parent)
{
    m_tabWidget = new QTabWidget(this);
    // 创建查找窗口
    m_searchWindow = new GLDSearchWindow(this);

    // 创建错误窗口
    m_errorWindow = new GLDErrorWindow(this);

    // 创建第三个页面
    m_quoteWindow        = new QWidget(this);
    QVBoxLayout* layout3 = new QVBoxLayout;
    QLabel*      label3  = new QLabel("This is the content of Tab 3");
    layout3->addWidget(label3);

    m_quoteWindow->setLayout(layout3);

    // 将页面添加到 QTabWidget 中
    m_tabWidget->addTab(m_searchWindow, "Search");
    m_tabWidget->addTab(m_errorWindow, "Error");
    m_tabWidget->addTab(m_quoteWindow, "Quote");

    // 设置布局
    setAllowedAreas(Qt::AllDockWidgetAreas);
    setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetClosable);
    setWidget(m_tabWidget);
}

void GLDAuxiliaryArea::setSearchResults(const QMap<int, QString>& results)
{
    m_searchWindow->setSearchResults(results);
}
