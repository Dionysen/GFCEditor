#ifndef GLDSchemaWidget_H
#define GLDSchemaWidget_H
#pragma once
#include "GFCReader.h"
#include "GLDTreeModel.h"

#include <QTabWidget>
#include <QTreeView>
#include <QStandardItemModel>

class GLDSchemaWidget : public QTabWidget
{
    Q_OBJECT

  public:
    GLDSchemaWidget(QWidget* parent = nullptr);
    ~GLDSchemaWidget();

  private:
    void createSchemaModel();
    void createProjectModel();
    void AddChildNode(QStandardItem* parent, QString item);

  private:
    QTreeView* p_schemaTreeView;
    QTreeView* p_projectTreeView;

    GLDTreeModel* p_schemaModel;
    GLDTreeModel* p_projectModel;

    GFCReader* p_GfcReader;
};

#endif  // GLDSchemaWidget_H
