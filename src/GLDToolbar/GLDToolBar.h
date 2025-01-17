﻿#ifndef GLDTOOLBAR_H
#define GLDTOOLBAR_H

#include <QToolBar>
#include <QToolButton>
#include <QWidget>
#include <QAction>
#include <QVBoxLayout>

class GLDToolBar : public QToolBar
{

    friend class MainWindow;
    Q_OBJECT
  public:
    GLDToolBar(QWidget* parent = nullptr);
    ~GLDToolBar();

    void setIcon();

  private:
    QAction* p_new;
    QAction* p_open;
    QAction* p_save;

    QAction* p_undo;
    QAction* p_redo;

    QAction* p_cut;
    QAction* p_copy;
    QAction* p_paste;

    QAction* p_find;
    QAction* p_replace;

    QAction* p_forward;
    QAction* p_backward;

    QAction* p_refresh;
};

#endif  // GLDTOOLBAR_H
