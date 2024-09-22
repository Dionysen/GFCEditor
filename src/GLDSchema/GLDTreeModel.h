#ifndef MYTREEMODEL_H
#define MYTREEMODEL_H

#include <QAbstractItemModel>
#include <QIcon>

class GLDTreeNode
{
  public:
    GLDTreeNode(GLDTreeNode* pParent = NULL);
    GLDTreeNode(const QString& content, GLDTreeNode* parent = nullptr);

    void addNode(GLDTreeNode* pNode);

    void removeNode(int nRow);

    int row();

    int          childCount() const;
    int          column();
    GLDTreeNode* child(int nRow) const;
    GLDTreeNode* parent();

    void  setIcon(const QIcon& icon);
    QIcon getIcon() const;

    bool insertNode(int nRow, GLDTreeNode* pNode);

  public:
    GLDTreeNode*          m_Parent;
    QVector<GLDTreeNode*> m_Children;
    QString               m_Content;
    QIcon                 m_Icon;
};

class GLDTreeModel : public QAbstractItemModel
{
    Q_OBJECT
  public:
    GLDTreeModel(QObject* parent = 0);
    ~GLDTreeModel();

    // Override
    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent) const;

    QModelIndex index(int row, int column, const QModelIndex& parent) const;
    QModelIndex parent(const QModelIndex& child) const;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

    // Modify Data
    bool setData(const QModelIndex& index, const QVariant& value, int role);

    virtual bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex());
    virtual bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex());



  public:
    void initModel();
    void initModel(GLDTreeNode* root);

  private:
    GLDTreeNode* m_RootItem;
};

#endif  // MYTREEMODEL_H