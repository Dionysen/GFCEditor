#include "GLDToolBar.h"
#include "qkeysequence.h"
#include <QIcon>
#include <QApplication>

GLDToolBar::GLDToolBar(QWidget* parent)
    : QToolBar(parent)
{

    p_new  = new QAction(QIcon(":/image/new.png"), QStringLiteral("新建"), this);
    p_open = new QAction(QIcon(":/image/open.png"), QStringLiteral("打开"), this);
    p_save = new QAction(QIcon(":/image/save.png"), QStringLiteral("保存"), this);

    p_undo = new QAction(QIcon(":/image/undo.png"), QStringLiteral("撤销"), this);
    p_redo = new QAction(QIcon(":/image/redo.png"), QStringLiteral("重做"), this);

    p_cut   = new QAction(QIcon(":/image/cut.png"), QStringLiteral("剪切"), this);
    p_copy  = new QAction(QIcon(":/image/copy.png"), QStringLiteral("复制"), this);
    p_paste = new QAction(QIcon(":/image/paste.png"), QStringLiteral("粘贴"), this);

    p_find    = new QAction(QIcon(":/image/find.png"), QStringLiteral("查找"), this);
    p_replace = new QAction(QIcon(":/image/replace.png"), QStringLiteral("替换"), this);

    p_forward  = new QAction(QIcon(":/image/forward.png"), QStringLiteral("前进"), this);
    p_backward = new QAction(QIcon(":/image/backward.png"), QStringLiteral("后退"), this);

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
    this->setStyleSheet("QToolBar { icon-size: 20px; }");
}
GLDToolBar::~GLDToolBar()
{
}
