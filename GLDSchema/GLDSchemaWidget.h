
#include "qobjectdefs.h"
#include "qtabwidget.h"
#include "qtreeview.h"
#include <QWidget>
#include <QHBoxLayout>

class GLDSchemaWidget : public QWidget
{
    Q_OBJECT

  public:
    GLDSchemaWidget(QWidget* parent = nullptr);
    ~GLDSchemaWidget();

  private:
    QTabWidget*  p_tabWidget;
    QTreeView*   p_schemaTreeView;
    QTreeView*   p_projectTreeView;
    QHBoxLayout* p_mainLayout;
};
