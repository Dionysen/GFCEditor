#include "GLDToolBar.h"
#include <QIcon>
#include <QApplication>
#include <QStyle>

GLDToolBar::GLDToolBar(QWidget* parent)
    : QToolBar(parent)
{

    p_new      = new QAction(QStringLiteral("新建"), this);
    p_open     = new QAction(QStringLiteral("打开"), this);
    p_save     = new QAction(QStringLiteral("保存"), this);
    p_undo     = new QAction(QStringLiteral("撤销"), this);
    p_redo     = new QAction(QStringLiteral("重做"), this);
    p_cut      = new QAction(QStringLiteral("剪切"), this);
    p_copy     = new QAction(QStringLiteral("复制"), this);
    p_paste    = new QAction(QStringLiteral("粘贴"), this);
    p_find     = new QAction(QStringLiteral("查找"), this);
    p_replace  = new QAction(QStringLiteral("替换"), this);
    p_forward  = new QAction(QStringLiteral("前进"), this);
    p_backward = new QAction(QStringLiteral("后退"), this);

    p_refresh = new QAction("refresh", this);


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

    this->setStyleSheet("QToolBar { icon-size: 20px; }");
}
GLDToolBar::~GLDToolBar()
{
}
