#include "GLDSchemaWidget.h"
#include <QSizePolicy>

GLDSchemaWidget::GLDSchemaWidget(const QString& title, QWidget* parent)
    : QDockWidget(title, parent)
{
    p_GfcReader = new GFCReader;
    // p_GfcReader->LoadExpressFile("./res/GFC3X2.exp");

    p_tabWidget = new QTabWidget(this);
    // Schema
    p_schemaTreeView = new QTreeView(this);

    p_schemaTreeView->setHeaderHidden(true);
    p_schemaTreeView->setFocusPolicy(Qt::NoFocus);
    // 创建根节点
    p_schemaModel = new GLDTreeModel(this);

    createSchemaModel();

    p_schemaTreeView->setModel(p_schemaModel);
    p_schemaTreeView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    p_schemaTreeView->collapseAll();
    p_schemaTreeView->setUniformRowHeights(true);  // 优化性能

    p_projectModel = new QStandardItemModel(this);
    // Project
    p_projectTreeView = new QTreeView(this);
    p_projectTreeView->setModel(p_projectModel);
    p_projectTreeView->setHeaderHidden(true);
    p_projectTreeView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    p_projectTreeView->setUniformRowHeights(true);  // 优化性能

    p_tabWidget->addTab(p_schemaTreeView, QString("Schema"));
    p_tabWidget->addTab(p_projectTreeView, QString("Project"));

    p_tabWidget->setTabPosition(QTabWidget::South);

    setAllowedAreas(Qt::AllDockWidgetAreas);
    setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetClosable);
    setWidget(p_tabWidget);
}

void GLDSchemaWidget::createSchemaModel()
{
    // auto data = p_GfcReader->GetExpressData();

    // QStandardItem* rootItem = p_schemaModel;

    // for (auto it = data.begin(); it != data.end(); ++it)
    // {
    //     if (it.value()->GetTypeName() != "ENTITY")
    //     {
    //         continue;
    //     }

    //     // 添加父类
    //     QStandardItem* parentItem = new QStandardItem(QIcon(":/icon/dark/branch.svg"), it.value()->GetName());
    //     p_schemaModel->insertRow(parentItem);

    //     // 添加父类实例
    //     for (auto i : it.value().dynamicCast<EntityData>()->GetEntities())
    //     {
    //         QStandardItem* entityItem = new QStandardItem(QIcon(":/icon/dark/attribute.svg"), i.GetNumber() + " " + i.GetName());
    //         parentItem->appendRow(entityItem);
    //     }

    //     // 添加子类
    //     QStringList subTypes = it.value().dynamicCast<EntityData>()->GetSubTypes();
    //     for (auto subType : subTypes)
    //     {
    //         QStandardItem* childItem = new QStandardItem(QIcon(":/icon/dark/attribute.svg"), subType);
    //         parentItem->appendRow(childItem);
    //     }
    // }
}

void GLDSchemaWidget::AddChildNode(QStandardItem* parent, QString item)
{
    QStandardItem* childItem = new QStandardItem(item);
    parent->appendRow(childItem);
}

GLDSchemaWidget::~GLDSchemaWidget()
{
}
