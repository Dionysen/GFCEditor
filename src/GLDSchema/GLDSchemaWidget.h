#include <QStandardItemModel>
#include <QTabWidget>
#include <QTreeView>
#include <QWidget>
#include <QHBoxLayout>

#include "GFCReader.h"
#include "GLDTreeModel.h"
#include "qdockwidget.h"

class GLDSchemaWidget : public QDockWidget
{
    Q_OBJECT

  public:
    GLDSchemaWidget(const QString& title, QWidget* parent = nullptr);
    ~GLDSchemaWidget();

  private:
    void createSchemaModel();
    void createProjectModel();
    void AddChildNode(QStandardItem* parent, QString item);


  private:
  private:
    QTabWidget* p_tabWidget;

    QTreeView* p_schemaTreeView;
    QTreeView* p_projectTreeView;

    GLDTreeModel* p_schemaModel;
    GLDTreeModel* p_projectModel;

    GFCReader* p_GfcReader;
};
