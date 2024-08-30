#include "GLDErrorWindow.h"
#include <QVBoxLayout>
#include <QListWidget>
#include <qDebug>

GLDErrorWindow::GLDErrorWindow(QWidget* parent)
    : QWidget(parent)
{
    // 垂直布局（用于容纳所有控件）
    QVBoxLayout* mainLayout = new QVBoxLayout;

    // 合法性结果列表
    m_pResultList = new QListWidget;

    mainLayout->addWidget(m_pResultList);
    mainLayout->setMargin(0);
    setLayout(mainLayout);

    // 设置样式表：突出显示当前行，发送行号
    connect(m_pResultList, &QListWidget::currentRowChanged, this, [=](int currentRow) {
        for (int i = 0; i < m_pResultList->count(); ++i)
        {
            if (i == currentRow)
            {
                m_pResultList->item(i)->setBackground(Qt::red);
            }
            else
            {
                m_pResultList->item(i)->setBackground(Qt::white);
            }
        }

        // 发送行号
        for(int i=0; i<m_RowsList.size(); i++)
        {
            qDebug() << "The value is:" << m_RowsList[i];
        }
        emit signalClickRow(m_RowsList[currentRow]);
    });
}


void GLDErrorWindow::updateSearchResults(const QMap<int, QString>& results)
{
    m_pResultList->clear(); // 清除之前的结果
    m_RowsList.clear();

    for(auto it = results.begin(); it != results.end(); ++it)
    {
        m_RowsList.append(it.key());  // 收集行号
        QListWidgetItem *item = new QListWidgetItem(QIcon("../bigwork/src/error.png"), it.value());
        m_pResultList->addItem(item);
    }
}


