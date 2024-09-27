#include "GLDSchemaWidget.h"
#include "GLDTreeModel.h"
#include <QSizePolicy>

GLDSchemaWidget::GLDSchemaWidget(const QString& title, QWidget* parent)
    : QDockWidget(title, parent)
{
    p_GfcReader = new GFCReader;
    p_GfcReader->LoadExpressFile("./res/GFC3X4.exp");
    p_GfcReader->LoadGfcFile("./res/test.gfc");
    // p_GfcReader->PrintGfc();

    p_tabWidget = new QTabWidget(this);


    p_schemaModel = new GLDTreeModel(this);
    createSchemaModel();

    p_schemaTreeView = new QTreeView(this);
    p_schemaTreeView->setHeaderHidden(true);
    p_schemaTreeView->setFocusPolicy(Qt::NoFocus);
    p_schemaTreeView->setIndentation(15);
    p_schemaTreeView->setModel(p_schemaModel);
    p_schemaTreeView->expandToDepth(3);
    p_schemaTreeView->setUniformRowHeights(true);  // 优化性能

    // Project
    p_projectModel = new GLDTreeModel(this);
    createProjectModel();

    p_projectTreeView = new QTreeView(this);
    p_projectTreeView->setModel(p_projectModel);
    p_projectTreeView->setHeaderHidden(true);
    p_projectTreeView->setUniformRowHeights(true);  // 优化性能
    p_projectTreeView->expandToDepth(3);

    p_tabWidget->addTab(p_schemaTreeView, QString("Schema"));
    p_tabWidget->addTab(p_projectTreeView, QString("Project"));
    p_tabWidget->setTabPosition(QTabWidget::South);

    // Docking
    setAllowedAreas(Qt::AllDockWidgetAreas);
    setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetClosable);
    setWidget(p_tabWidget);
}

void GLDSchemaWidget::createSchemaModel()
{
    auto&        data = p_GfcReader->GetExpressData();
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
            childItem->setIcon(entityIcon);
            parentItem->addNode(childItem);

            // 添加子类的实例
            if (data.contains(subType.toUpper()))
            {
                auto entities = data[subType.toUpper()].dynamicCast<EntityData>()->GetEntities();
                for (auto entity : entities)
                {
                    GLDTreeNode* childEntityItem = new GLDTreeNode(entity.GetName(), childItem);
                    childEntityItem->setIcon(attributeIcon);
                    childItem->addNode(childEntityItem);
                    auto attributes = entity.GetAttributes();
                    for (auto attribute : attributes)
                    {
                        GLDTreeNode* childAttributeItem = new GLDTreeNode(attribute.m_Name + " = " + attribute.m_Value, childEntityItem);
                        childAttributeItem->setIcon(attributeIcon);
                        childEntityItem->addNode(childAttributeItem);
                    }
                }
            }
        }
    }
    p_schemaModel->initModel(root);
}

void GLDSchemaWidget::createProjectModel()
{
    auto&        data = p_GfcReader->GetExpressData();
    GLDTreeNode* root = new GLDTreeNode("Root", nullptr);
    // // Project
    // if (!data.contains("GFCPROJECT"))
    //     return;

    // AddProjectNode(root, "GFCPROJECT");

    // auto gfcProject = data.value("GFCPROJECT").dynamicCast<EntityData>();
    // auto entities   = gfcProject->GetEntities();
    // for (auto& entity : entities)
    // {
    //     GLDTreeNode* projItem = new GLDTreeNode(entity.GetName(), root);
    //     root->addNode(projItem);

    //     // Site
    //     if (data.contains("GFCSITE"))
    //     {
    //         auto site  = data.value("GFCSITE").dynamicCast<EntityData>();
    //         auto sites = site->GetEntities();
    //         for (auto& site : sites)
    //         {
    //             GLDTreeNode* siteItem = new GLDTreeNode(site.GetName(), projItem);
    //             projItem->addNode(siteItem);
    //         }
    //     }
    //     else
    //     {
    //         // Building
    //         if (!data.contains("GFCBUILDING"))
    //             continue;
    //         auto building  = data.value("GFCBUILDING").dynamicCast<EntityData>();
    //         auto buildings = building->GetEntities();
    //         for (auto& building : buildings)
    //         {
    //             GLDTreeNode* buildItem = new GLDTreeNode(building.GetName(), projItem);
    //             projItem->addNode(buildItem);
    //         }
    //     }
    // }


    p_projectModel->initModel(root);
}

void GLDSchemaWidget::AddChildNode(QStandardItem* parent, QString item)
{
    QStandardItem* childItem = new QStandardItem(item);
    parent->appendRow(childItem);
}



GLDSchemaWidget::~GLDSchemaWidget()
{
}
