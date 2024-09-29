#include "GLDAuxiliaryArea.h"
#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QLabel>
#include "GLDSearchWindow.h"
#include "GLDErrorWindow.h"

GLDAuxiliaryArea::GLDAuxiliaryArea(QWidget* parent)
    : QTabWidget(parent)
{
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
    addTab(m_searchWindow, "Search");
    addTab(m_errorWindow, "Error");
    addTab(m_quoteWindow, "Quote");
}

void GLDAuxiliaryArea::setSearchResults(const QMap<int, QString>& results)
{
    m_searchWindow->setSearchResults(results);
}
