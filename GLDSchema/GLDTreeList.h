#ifndef MYTREEMODEL_H
#define MYTREEMODEL_H

#include "qicon.h"
#include "qobject.h"
#include <QAbstractItemModel>
#include <QIcon>
class GLDTreeNode
{
  public:
    GLDTreeNode(GLDTreeNode* pParent = NULL);

    void addNode(GLDTreeNode* pNode);
    void removeNode(int nRow);

    int row();
    int childCount();
    int column();

    GLDTreeNode* child(int nRow);
    GLDTreeNode* parent();

    bool insertNode(int nRow, GLDTreeNode* pNode);

  public:
    GLDTreeNode*          m_pParent;
    QVector<GLDTreeNode*> m_lstChildren;

    QSharedPointer<QIcon> m_Icon;
    QString               m_className;
    QList<QString>        m_properties;
};
class GLDSchemaTreeModel : public QAbstractItemModel
{
    Q_OBJECT
  public:
    GLDSchemaTreeModel(QObject* parent = 0);
    ~GLDSchemaTreeModel();

    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent) const;

    QModelIndex index(int row, int column, const QModelIndex& parent) const;
    QModelIndex parent(const QModelIndex& child) const;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

    bool setData(const QModelIndex& index, const QVariant& value, int role);

    virtual Qt::ItemFlags flags(const QModelIndex& index) const;

    virtual bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex());
    virtual bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex());

  private:
    void initModel();

  private:
    QSharedPointer<QIcon> m_fileIcon;
    QSharedPointer<QIcon> m_nodeIcon;
    QSharedPointer<QIcon> m_propIcon;
    GLDTreeNode*          m_pDataSource;
};

#endif  // MYTREEMODEL_H
