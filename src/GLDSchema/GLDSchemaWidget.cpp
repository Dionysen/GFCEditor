#include "GLDSchemaWidget.h"
#include "GLDTreeModel.h"
#include <QSizePolicy>

GLDSchemaWidget::GLDSchemaWidget(const QString& title, QWidget* parent)
    : QDockWidget(title, parent)
{
    p_GfcReader = new GFCReader;
    p_GfcReader->LoadExpressFile("./res/GFC3X2.exp");

    p_tabWidget = new QTabWidget(this);
    // Schema
    p_schemaTreeView = new QTreeView(this);

    p_schemaTreeView->setHeaderHidden(true);
    p_schemaTreeView->setFocusPolicy(Qt::NoFocus);
    p_schemaTreeView->setIndentation(15);


    p_schemaModel = new GLDTreeModel(this);
    // 设置子节点缩进距离
    createSchemaModel();

    p_schemaTreeView->setModel(p_schemaModel);
    p_schemaTreeView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    p_schemaTreeView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    p_schemaTreeView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    p_schemaTreeView->expandToDepth(0);
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
    auto data = p_GfcReader->GetExpressData();

    GLDTreeNode* root = new GLDTreeNode("Root", nullptr);

    // 预先加载图标
    QIcon attributeIcon(":/icon/dark/attribute.svg");
    QIcon entityIcon(":/icon/dark/schema.svg");

    for (auto it = data.begin(); it != data.end(); ++it)
    {
        if (it.value()->GetTypeName() != "ENTITY")
        {
            continue;
        }
        // 添加父类
        GLDTreeNode* parentItem = new GLDTreeNode(it.value()->GetName(), root);
        parentItem->setIcon(entityIcon);
        root->addNode(parentItem);

        // 添加子类
        QStringList subTypes = it.value().dynamicCast<EntityData>()->GetSubTypes();
        for (auto subType : subTypes)
        {
            GLDTreeNode* childItem = new GLDTreeNode(subType, parentItem);
            childItem->setIcon(attributeIcon);
            parentItem->addNode(childItem);
        }
    }
    p_schemaModel->initModel(root);
}

void GLDSchemaWidget::AddChildNode(QStandardItem* parent, QString item)
{
    QStandardItem* childItem = new QStandardItem(item);
    parent->appendRow(childItem);
}

GLDSchemaWidget::~GLDSchemaWidget()
{
}
