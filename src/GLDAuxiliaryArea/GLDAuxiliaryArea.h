#ifndef GLDAUXILIARYAREA_H
#define GLDAUXILIARYAREA_H
#pragma once
#include <QDockWidget>
#include <QTabWidget>
#include "GLDSearchWindow.h"
#include "GLDErrorWindow.h"


class GLDAuxiliaryArea : public QTabWidget
{
    Q_OBJECT
  public:
    GLDAuxiliaryArea(QWidget* parent = nullptr);

    // 更新搜索结果的函数（供外部调用）
    void setSearchResults(const QMap<int, QString>& results);

    // 发送内部信号到最外层TODO
    // private:
    GLDSearchWindow* m_searchWindow;  // 查找结果
    GLDErrorWindow*  m_errorWindow;   // 错误列表
    QWidget*         m_quoteWindow;   // 引用实例情况
};

#endif  // GLDAUXILIARYAREA_H
