#include "GLDAttributeArea.h"
#include <QWidget>
#include "GLDAttributeModel.h"

GLDAttributeArea::GLDAttributeArea(QWidget* parent)
    :QWidget(parent)
{
    // 创建 QTableView 和 QStandardItemModel
    m_pTableView = new QTableView(this);
    m_pModel = new GLDAttributeModel();

    // 设置 QTableView 的模型
    m_pTableView->setModel(m_pModel);

    // 创建布局并添加 QTableView
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_pTableView);
    setLayout(layout);

    // 连接tableview点击信号到发射信号传值的槽函数
    connect(m_pTableView, &QTableView::clicked, this, [=](const QModelIndex &index)
    {
        // 获取第2列的索引
        QModelIndex secondColumnIndex = index.sibling(index.row(), 1);
        QString value = m_pModel->data(secondColumnIndex, Qt::EditRole).toString();

        emit signalClickTableView(value);
    });

}

// 接收外部传入的数据并更新表格
void GLDAttributeArea::setProperties(const QMap<QString, QVariant>& properties) {
    m_pModel->setProperties(properties);
}
