#include "GLDAttributeArea.h"
#include <QWidget>
#include <QHeaderView>

GLDAttributeArea::GLDAttributeArea(const QString& title, QWidget* parent)
    : QDockWidget(title, parent)
{
    // 创建 QTableView 和 QStandardItemModel
    m_pTableView = new QTableView(this);
    m_pModel     = new QStandardItemModel();

    // 设置 QTableView 的模型
    m_pTableView->setModel(m_pModel);

    setAllowedAreas(Qt::AllDockWidgetAreas);
    setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetClosable);
    setWidget(m_pTableView);

    // 设置选择方式为行
    m_pTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    // 隐藏表头
    m_pTableView->verticalHeader()->setVisible(false);

    // 设置表格随布局大小改变
    m_pTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 取消选中后的虚线框
    m_pTableView->setFocusPolicy(Qt::NoFocus);


    // 连接tableview点击信号到发射信号传值的槽函数
    connect(m_pTableView, &QTableView::clicked, this, [=](const QModelIndex& index) {
        // 获取第2列的索引
        QModelIndex secondColumnIndex = index.sibling(index.row(), 1);
        QString     value             = m_pModel->data(secondColumnIndex, Qt::EditRole).toString();

        emit signalClickTableView(value);
    });
}

// 接收外部传入的数据并更新表格
void GLDAttributeArea::setProperties(const QMap<QString, QVariant>& properties)
{
    // 清空现有的模型数据
    m_pModel->clear();

    // 设置表头
    m_pModel->setHorizontalHeaderLabels({ "Attribute", "Value" });

    // 遍历属性映射并添加到模型中
    int row = 0;
    for (auto it = properties.constBegin(); it != properties.constEnd(); ++it)
    {
        QStandardItem* keyItem   = new QStandardItem(it.key());
        QStandardItem* valueItem = new QStandardItem(it.value().toString());

        m_pModel->setItem(row, 0, keyItem);
        m_pModel->setItem(row, 1, valueItem);

        row++;
    }

    // 调整列宽以适应内容
    m_pTableView->resizeColumnsToContents();
}
