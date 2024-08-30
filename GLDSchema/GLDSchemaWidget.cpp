#include "GLDSchemaWidget.h"
#include "qchar.h"
#include "qtabwidget.h"
#include "qwidget.h"
#include "GLDTreeList.h"
#include <QSizePolicy>
#include "GLDEntity.h"
#include <QStandardItemModel>

GLDSchemaWidget::GLDSchemaWidget(QWidget* parent)
    : QWidget(parent)
{
    // 解析Exp文件
    QString path{ "C:/Users/glodon/2024-cpp5/bigwork/Resources/GFC3X2.exp" };
    GLDEntity::parseExp(path.toStdString());
    std::map<int, std::pair<QString, QString>> temp = readGBKFile("C:/Users/glodon/2024-cpp5/bigwork/Resources/P0P1-origin.gfc");
    GLDExpPraseInfo::parseFileGetInstance(temp);

    p_tabWidget = new QTabWidget(this);
    // Schema
    p_schemaTreeView = new QTreeView(this);

    p_schemaTreeView->setHeaderHidden(true);
    p_schemaTreeView->setModel(new GLDSchemaTreeModel(p_schemaTreeView));
    p_schemaTreeView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    p_schemaTreeView->expandToDepth(0);


    // Project
    p_projectTreeView = new QTreeView(this);
    p_projectTreeView->setModel(new GLDSchemaTreeModel(p_schemaTreeView));
    p_projectTreeView->setHeaderHidden(true);
    p_projectTreeView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    p_tabWidget->addTab(p_schemaTreeView, QString("Schema"));
    p_tabWidget->addTab(p_projectTreeView, QString("Project"));

    p_tabWidget->setTabPosition(QTabWidget::South);
    // 控件布局
    p_mainLayout = new QHBoxLayout;
    p_mainLayout->addWidget(p_tabWidget);
    p_mainLayout->setMargin(0);
    setLayout(p_mainLayout);
}
GLDSchemaWidget::~GLDSchemaWidget()
{
}
