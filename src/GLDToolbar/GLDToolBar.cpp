#include "GLDToolBar.h"
#include "qicon.h"
#include <QIcon>
#include <QApplication>
#include <QStyle>

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



    p_new->setIcon(QIcon(":/icon/dark/new.svg"));
    p_open->setIcon(QIcon(":/icon/dark/open.svg"));
    p_save->setIcon(QIcon(":/icon/dark/save.svg"));
    p_undo->setIcon(QIcon(":/icon/dark/undo.svg"));
    p_redo->setIcon(QIcon(":/icon/dark/redo.svg"));
    p_cut->setIcon(QIcon(":/icon/dark/cut.svg"));
    p_copy->setIcon(QIcon(":/icon/dark/copy.svg"));
    p_paste->setIcon(QIcon(":/icon/dark/paste.svg"));
    p_find->setIcon(QIcon(":/icon/dark/find.svg"));
    p_replace->setIcon(QIcon(":/icon/dark/replace.svg"));
    p_forward->setIcon(QIcon(":/icon/dark/forward.svg"));
    p_backward->setIcon(QIcon(":/icon/dark/backward.svg"));


    p_refresh = new QAction(this);
    p_refresh->setIcon(QIcon(":/icon/dark/refresh.svg"));

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
}
GLDToolBar::~GLDToolBar()
{
}
