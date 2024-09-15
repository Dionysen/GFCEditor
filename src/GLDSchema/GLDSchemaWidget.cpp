#include "GLDSchemaWidget.h"
#include "ExpressData.h"
#include "qchar.h"
#include "qstandarditemmodel.h"
#include "qtabwidget.h"
#include "qwidget.h"
#include <QSizePolicy>
#include <QStandardItemModel>

GLDSchemaWidget::GLDSchemaWidget(QWidget* parent)
    : QWidget(parent)
{
    p_GfcReader = new GFCReader;
    p_GfcReader->LoadExpressFile("./resources/GFC3X2.exp");

    p_tabWidget = new QTabWidget(this);
    // Schema
    p_schemaTreeView = new QTreeView(this);

    p_schemaTreeView->setHeaderHidden(true);

    // 创建根节点
    p_schemaModel = new QStandardItemModel(this);

    createSchemaModel();
    p_schemaTreeView->setModel(p_schemaModel);
    p_schemaTreeView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // p_schemaTreeView->expandToDepth(0);
    p_schemaTreeView->collapseAll();
    // p_schemaTreeView->setEditTriggers(false);

    p_projectModel = new QStandardItemModel(this);
    // Project
    p_projectTreeView = new QTreeView(this);
    p_projectTreeView->setModel(p_projectModel);
    p_projectTreeView->setHeaderHidden(true);
    p_projectTreeView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    p_tabWidget->addTab(p_schemaTreeView, QString("Schema"));
    p_tabWidget->addTab(p_projectTreeView, QString("Project"));

    p_tabWidget->setTabPosition(QTabWidget::South);
    // 控件布局
    p_mainLayout = new QHBoxLayout;
    p_mainLayout->addWidget(p_tabWidget);
    p_mainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(p_mainLayout);
}

void GLDSchemaWidget::createSchemaModel()
{
    auto data = p_GfcReader->GetExpressData();

    QStandardItem* rootItem = p_schemaModel->invisibleRootItem();

    for (auto it = data.begin(); it != data.end(); ++it)
    {
        if (it.value()->GetTypeName() != "ENTITY")
        {
            continue;
        }

        // 添加父类
        QStandardItem* parentItem = new QStandardItem(it.value()->GetName());
        rootItem->appendRow(parentItem);

        // 添加父类实例
        for (auto i : it.value().dynamicCast<EntityData>()->GetEntities())
        {
            QStandardItem* entityItem = new QStandardItem(i.GetNumber() + " " + i.GetName());
            parentItem->appendRow(entityItem);
        }


        // 添加子类
        QStringList subTypes = it.value().dynamicCast<EntityData>()->GetSubTypes();
        for (auto subType : subTypes)
        {
            QStandardItem* childItem = new QStandardItem(subType);
            parentItem->appendRow(childItem);
        }
    }
}

void GLDSchemaWidget::AddChildNode(QStandardItem* parent, QString item)
{
    QStandardItem* childItem = new QStandardItem(item);
    parent->appendRow(childItem);
}

GLDSchemaWidget::~GLDSchemaWidget()
{
}
