#include "GLDTreeModel.h"
#include "qcontainerfwd.h"
#include "qobject.h"

// ========= TreeItem ==============


// ========= TreeModel ==============
GLDTreeModel::GLDTreeModel(QObject* parent)
    : QAbstractItemModel(parent)
{
    QString rootData = "start";
    rootItem         = new GLDTreeItem(rootData);
    setupModelData(rootItem);
}
GLDTreeModel::~GLDTreeModel()
{
}


QVariant GLDTreeModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    GLDTreeItem* item = static_cast<GLDTreeItem*>(index.internalPointer());

    return item->data(index.column());
}


QModelIndex GLDTreeModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();
    // 获取父节点
    GLDTreeItem* parentItem;
    // 如果父节点无效，则使用根节点
    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<GLDTreeItem*>(parent.internalPointer());
    // 获取子节点
    GLDTreeItem* childItem = parentItem->child(row);
    // 如果子节点有效，则返回子节点的索引
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex GLDTreeModel::parent(const QModelIndex& index) const
{
    if (!index.isValid())
        return QModelIndex();

    GLDTreeItem* childItem  = static_cast<GLDTreeItem*>(index.internalPointer());
    GLDTreeItem* parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int GLDTreeModel::rowCount(const QModelIndex& parent) const
{
    if (parent.column() > 0)
        return 0;

    GLDTreeItem* parentItem;
    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<GLDTreeItem*>(parent.internalPointer());

    if (parentItem == nullptr)
        return 0;

    return parentItem->childCount();
}

int GLDTreeModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return static_cast<GLDTreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

bool GLDTreeModel::hasChildren(const QModelIndex& parent) const
{
    if (!parent.isValid())
        return true;

    GLDTreeItem* parentItem = getItem(parent);
    return !parentItem->isChildrenLoaded() || parentItem->childCount() > 0;
}

bool GLDTreeModel::canFetchMore(const QModelIndex& parent) const
{
    if (!parent.isValid())
        return false;

    GLDTreeItem* parentItem = getItem(parent);
    return !parentItem->isChildrenLoaded();
}

void GLDTreeModel::fetchMore(const QModelIndex& parent)
{
    if (!parent.isValid())
        return;

    GLDTreeItem* parentItem = getItem(parent);
    if (parentItem->isChildrenLoaded())
        return;

    // 这里加载子项目
    beginInsertRows(parent, 0, 4);  // 假设我们要添加5个子项
    for (int i = 0; i < 5; ++i)
    {
        QString data;
        data = QString("子项 %1").arg(i) + "描述";
        parentItem->appendChild(new GLDTreeItem(data, parentItem));
    }
    endInsertRows();

    parentItem->setChildrenLoaded(true);
}

void GLDTreeModel::setupModelData(GLDTreeItem* parent)
{
    // 只加载顶层项目
    QString data;
    data = QString("项目1") + "描述1";
    parent->appendChild(new GLDTreeItem(data, parent));

    QString data2;
    data = QString("项目2") + "描述2";
    parent->appendChild(new GLDTreeItem(data, parent));
}

GLDTreeItem* GLDTreeModel::getItem(const QModelIndex& index) const
{
    if (index.isValid())
    {
        GLDTreeItem* item = static_cast<GLDTreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}