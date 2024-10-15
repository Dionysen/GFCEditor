#include "GLDTreeModel.h"


GLDTreeNode::GLDTreeNode(GLDTreeNode* pParent)
    : m_Parent(pParent)
    , m_Content("")
{
}

GLDTreeNode::GLDTreeNode(const QString& content, GLDTreeNode* parent)
    : m_Parent(parent)
    , m_Content(content)
{
}

void GLDTreeNode::setIcon(const QIcon& icon)
{
    m_Icon = icon;
}

QIcon GLDTreeNode::getIcon() const
{
    return m_Icon;
}

void GLDTreeNode::addNode(GLDTreeNode* pNode)
{
    m_Children.append(pNode);
}

void GLDTreeNode::removeNode(int nRow)
{
    if (nRow < 0 || nRow >= m_Children.count())
    {
        return;
    }

    m_Children.remove(nRow);
}

int GLDTreeNode::row()
{
    if (nullptr == m_Parent)
    {
        return 0;
    }

    return m_Parent->m_Children.indexOf(this);
}

int GLDTreeNode::childCount() const
{
    return m_Children.count();
}

int GLDTreeNode::column()
{
    return 1;
}

GLDTreeNode* GLDTreeNode::child(int nRow) const
{
    if (nRow < 0 || nRow >= m_Children.count())
    {
        return nullptr;
    }

    return m_Children.at(nRow);
}

GLDTreeNode* GLDTreeNode::parent()
{
    return m_Parent;
}

bool GLDTreeNode::insertNode(int nRow, GLDTreeNode* pNode)
{
    if (nRow < 0 || nRow > m_Children.count())
    {
        return false;
    }

    m_Children.insert(nRow, pNode);
    return true;
}


GLDTreeModel::GLDTreeModel(QObject* parent)
    : QAbstractItemModel(parent)
    , m_RootItem(nullptr)
{
    // initModel();
}

GLDTreeModel::~GLDTreeModel()
{
}

int GLDTreeModel::rowCount(const QModelIndex& parent) const
{
    if (!parent.isValid())
        return 1;

    GLDTreeNode* pNode = static_cast<GLDTreeNode*>(parent.internalPointer());
    if (pNode)
    {
        return pNode->childCount();
    }

    return 0;
}

int GLDTreeModel::columnCount(const QModelIndex& parent) const
{
    if (!parent.isValid())
        return 1;

    GLDTreeNode* pNode = static_cast<GLDTreeNode*>(parent.internalPointer());
    if (pNode)
    {
        return pNode->column();
    }

    return 0;
}

QModelIndex GLDTreeModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!parent.isValid())
    {
        return createIndex(0, column, m_RootItem);
    }

    GLDTreeNode* p_ParentNode = static_cast<GLDTreeNode*>(parent.internalPointer());
    GLDTreeNode* p_Node       = nullptr;
    if (p_ParentNode)
    {
        p_Node = p_ParentNode->child(row);
    }

    return createIndex(row, column, p_Node);
}

QModelIndex GLDTreeModel::parent(const QModelIndex& child) const
{
    if (!child.isValid())
    {
        return QModelIndex();
    }

    GLDTreeNode* pChildNode  = static_cast<GLDTreeNode*>(child.internalPointer());
    GLDTreeNode* pParentNode = m_RootItem;
    if (pChildNode)
    {
        pParentNode = pChildNode->parent();
    }

    if (pParentNode)
        return createIndex(pParentNode->row(), 0, pParentNode);
    else
        return QModelIndex();
}

QVariant GLDTreeModel::data(const QModelIndex& index, int role) const
{
    QVariant varRet;
    if (!index.isValid())
        return varRet;

    GLDTreeNode* pNode = static_cast<GLDTreeNode*>(index.internalPointer());

    if (nullptr == pNode)
        return varRet;

    switch (role)
    {
    case Qt::DisplayRole: {
        return QVariant::fromValue(pNode->m_Content);
    }
    case Qt::DecorationRole: {
        return QVariant::fromValue(pNode->getIcon());
    }
    }
    return varRet;
}

bool GLDTreeModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid())
        return false;

    GLDTreeNode* pNode = static_cast<GLDTreeNode*>(index.internalPointer());
    if (nullptr == pNode)
        return false;

    switch (role)
    {
    case Qt::DisplayRole: {
        pNode->m_Content = value.toString();
    }
    break;
    default:
        return QAbstractItemModel::setData(index, value, role);
    }
    emit dataChanged(index, index);

    return QAbstractItemModel::setData(index, value, role);
}

bool GLDTreeModel::insertRows(int row, int count, const QModelIndex& parent)
{
    beginInsertRows(parent, row, row + count);
    GLDTreeNode* pParent = m_RootItem;
    if (parent.isValid())
    {
        pParent = static_cast<GLDTreeNode*>(parent.internalPointer());
    }

    for (int i = 0; i < count; ++i)
    {
        GLDTreeNode* pNode = new GLDTreeNode(pParent);
        pNode->m_Content   = "test";
        pParent->insertNode(row, pNode);
    }
    endInsertRows();
    return true;
}

bool GLDTreeModel::removeRows(int row, int count, const QModelIndex& parent)
{
    beginRemoveRows(parent, row, row + count);
    GLDTreeNode* pParent = m_RootItem;
    if (parent.isValid())
    {
        pParent = static_cast<GLDTreeNode*>(parent.internalPointer());
    }

    if (pParent)
    {
        for (int i = 0; i < count; ++i)
        {
            pParent->removeNode(row + i);
        }
    }
    endRemoveRows();
    return true;
}

void GLDTreeModel::initModel(GLDTreeNode* root)
{
    m_RootItem = root;
}


void GLDTreeModel::initModel()
{
    m_RootItem = new GLDTreeNode(nullptr);
    for (int i = 0; i < 10; ++i)
    {
        GLDTreeNode* pNode = new GLDTreeNode(m_RootItem);
        pNode->m_Content   = QString("Dept");
        m_RootItem->addNode(pNode);

        for (int j = 0; j < 5; ++j)
        {
            GLDTreeNode* pNode1 = new GLDTreeNode(pNode);
            pNode1->m_Content   = QString("Department");
            pNode->addNode(pNode1);
        }
    }
}
