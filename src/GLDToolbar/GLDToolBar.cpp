#include "GLDToolBar.h"

#include <QIcon>
#include <QApplication>
#include <QStyle>
#include "GLDStyleManager.h"

GLDToolBar::GLDToolBar(QWidget* parent)
    : QToolBar(parent)
{

    p_new      = new QAction(this);
    p_open     = new QAction(this);
    p_save     = new QAction(this);
    p_undo     = new QAction(this);
    p_redo     = new QAction(this);
    p_cut      = new QAction(this);
    p_copy     = new QAction(this);
    p_paste    = new QAction(this);
    p_find     = new QAction(this);
    p_replace  = new QAction(this);
    p_forward  = new QAction(this);
    p_backward = new QAction(this);

    p_refresh = new QAction(this);

    this->addAction(p_new);
    this->addAction(p_open);
    this->addAction(p_save);

    this->addSeparator();
    this->addAction(p_undo);
    this->addAction(p_redo);

    this->addSeparator();
    this->addAction(p_cut);
    this->addAction(p_copy);
    this->addAction(p_paste);

    this->addSeparator();
    this->addAction(p_find);
    this->addAction(p_replace);

    this->addSeparator();
    this->addAction(p_backward);
    this->addAction(p_forward);

    this->addAction(p_refresh);

    const int hight = 32;
    // 设置工具栏的固定高度和宽度
    this->setFixedHeight(hight);

    // 连接方向变化信号到槽函数
    connect(this, &QToolBar::orientationChanged, this, [this](Qt::Orientation orientation) {
        if (orientation == Qt::Horizontal)
        {
            this->setFixedHeight(hight);
            this->setFixedWidth(QWIDGETSIZE_MAX);
        }
        else
        {
            this->setFixedHeight(QWIDGETSIZE_MAX);
            this->setFixedWidth(hight);
        }
    });

    setIcon();
    connect(GLDStyleManager::StyleManager(), &GLDStyleManager::themeChanged, this, &GLDToolBar::setIcon);
}


void GLDToolBar::setIcon()
{
    p_new->setIcon(GLDStyleManager::StyleManager()->getIcon("new"));
    p_open->setIcon(GLDStyleManager::StyleManager()->getIcon("open"));
    p_save->setIcon(GLDStyleManager::StyleManager()->getIcon("save"));
    p_undo->setIcon(GLDStyleManager::StyleManager()->getIcon("undo"));
    p_redo->setIcon(GLDStyleManager::StyleManager()->getIcon("redo"));
    p_cut->setIcon(GLDStyleManager::StyleManager()->getIcon("cut"));
    p_copy->setIcon(GLDStyleManager::StyleManager()->getIcon("copy"));
    p_paste->setIcon(GLDStyleManager::StyleManager()->getIcon("paste"));
    p_find->setIcon(GLDStyleManager::StyleManager()->getIcon("find"));
    p_replace->setIcon(GLDStyleManager::StyleManager()->getIcon("replace"));
    p_forward->setIcon(GLDStyleManager::StyleManager()->getIcon("forward"));
    p_backward->setIcon(GLDStyleManager::StyleManager()->getIcon("backward"));

    p_refresh->setIcon(GLDStyleManager::StyleManager()->getIcon("refresh"));
}

GLDToolBar::~GLDToolBar()
{
}
