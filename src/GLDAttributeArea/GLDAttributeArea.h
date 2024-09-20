#ifndef GLDATTRIBUTEAREA_H
#define GLDATTRIBUTEAREA_H

#include <QDockWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include "qstandarditemmodel.h"


class GLDAttributeArea : public QDockWidget
{
    Q_OBJECT

  public:
    GLDAttributeArea(const QString& title, QWidget* parent = nullptr);


    // 接收外部传入的数据并更新表格
    void setProperties(const QMap<QString, QVariant>& properties);

  signals:
    // 点击某一行发送信号传属性值
    void signalClickTableView(const QString& value);

  private:
    QTableView*         m_pTableView;
    QStandardItemModel* m_pModel;
};

#endif  // GLDATTRIBUTEAREA_H
