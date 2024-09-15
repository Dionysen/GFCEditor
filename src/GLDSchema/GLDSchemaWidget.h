
#include "qchar.h"
#include "qobjectdefs.h"
#include "qstandarditemmodel.h"
#include "qtabwidget.h"
#include "qtreeview.h"
#include <QWidget>
#include <QHBoxLayout>
#include "GFCReader.h"

class GLDSchemaWidget : public QWidget
{
    Q_OBJECT

  public:
    GLDSchemaWidget(QWidget* parent = nullptr);
    ~GLDSchemaWidget();

  private:
    void createSchemaModel();
    void AddChildNode(QStandardItem* parent, QString item);

  private:
    QTabWidget* p_tabWidget;

    QTreeView* p_schemaTreeView;
    QTreeView* p_projectTreeView;

    QStandardItemModel* p_schemaModel;
    QStandardItemModel* p_projectModel;

    GFCReader* p_GfcReader;

    QHBoxLayout* p_mainLayout;
};
