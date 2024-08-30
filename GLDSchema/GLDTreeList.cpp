#include "GLDTreeList.h"
#include "GLDEntity.h"
#include "qchar.h"
#include "qicon.h"
#include "qobject.h"
#include <QColor>
#include <QSharedPointer>
#include <map>
#include <qDebug>
#include <string>
GLDTreeNode::GLDTreeNode(GLDTreeNode* pParent)
{
    m_pParent   = pParent;
    m_className = "All";
}

void GLDTreeNode::addNode(GLDTreeNode* pNode)
{
    m_lstChildren.append(pNode);
}

void GLDTreeNode::removeNode(int nRow)
{
    if (nRow < 0 || nRow >= m_lstChildren.count())
    {
        return;
    }

    m_lstChildren.remove(nRow);
}

int GLDTreeNode::row()
{
    if (NULL == m_pParent)
    {
        return 0;
    }

    return m_pParent->m_lstChildren.indexOf(this);
}

int GLDTreeNode::childCount()
{
    return m_lstChildren.count();
}

int GLDTreeNode::column()
{
    return 1;
}

GLDTreeNode* GLDTreeNode::child(int nRow)
{
    if (nRow < 0 || nRow >= m_lstChildren.count())
    {
        return NULL;
    }

    return m_lstChildren.at(nRow);
}

GLDTreeNode* GLDTreeNode::parent()
{
    return m_pParent;
}

bool GLDTreeNode::insertNode(int nRow, GLDTreeNode* pNode)
{
    if (nRow < 0 || nRow > m_lstChildren.count())
    {
        return NULL;
    }

    m_lstChildren.insert(nRow, pNode);
    return true;
}


GLDSchemaTreeModel::GLDSchemaTreeModel(QObject* parent)
    : QAbstractItemModel(parent)
    , m_pDataSource(NULL)
{
    m_fileIcon = QSharedPointer<QIcon>::create(QString(":/image/treeFile.png"));
    m_nodeIcon = QSharedPointer<QIcon>::create(QString(":/image/treeNode.png"));
    m_propIcon = QSharedPointer<QIcon>::create(QString(":/image/treeProperty.png"));

    initModel();
}

GLDSchemaTreeModel::~GLDSchemaTreeModel()
{
}

int GLDSchemaTreeModel::rowCount(const QModelIndex& parent) const
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

int GLDSchemaTreeModel::columnCount(const QModelIndex& parent) const
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

QModelIndex GLDSchemaTreeModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!parent.isValid())
    {
        return createIndex(0, column, m_pDataSource);
    }

    GLDTreeNode* pParentNode = static_cast<GLDTreeNode*>(parent.internalPointer());
    GLDTreeNode* pNode       = NULL;
    if (pParentNode)
    {
        pNode = pParentNode->child(row);
    }

    return createIndex(row, column, pNode);
}

QModelIndex GLDSchemaTreeModel::parent(const QModelIndex& child) const
{
    if (!child.isValid())
    {
        return QModelIndex();
    }

    GLDTreeNode* pChildNode  = static_cast<GLDTreeNode*>(child.internalPointer());
    GLDTreeNode* pParentNode = m_pDataSource;
    if (pChildNode)
    {
        pParentNode = pChildNode->parent();
    }

    if (pParentNode)
        return createIndex(pParentNode->row(), 0, pParentNode);
    else
        return QModelIndex();
}

QVariant GLDSchemaTreeModel::data(const QModelIndex& index, int role) const
{
    QVariant varRet;
    if (!index.isValid())
        return varRet;

    GLDTreeNode* pNode = static_cast<GLDTreeNode*>(index.internalPointer());

    if (NULL == pNode)
        return varRet;

    switch (role)
    {
    case Qt::DisplayRole:
        return QVariant::fromValue(pNode->m_className);
        return varRet;
    case Qt::DecorationRole:  // 使用 DecorationRole 来显示图标
        if (index.column() == 0)
            return *pNode->m_Icon;
        break;
    default:
        break;
    }

    return varRet;
}

bool GLDSchemaTreeModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid())
        return false;

    GLDTreeNode* pNode = static_cast<GLDTreeNode*>(index.internalPointer());
    if (NULL == pNode)
        return false;

    switch (role)
    {
    case Qt::DisplayRole:
        pNode->m_className = value.toString();

    case Qt::DecorationRole:

        *pNode->m_Icon = value.value<QIcon>();
        break;
    default:
        return QAbstractItemModel::setData(index, value, role);
    }

    emit dataChanged(index, index);

    return QAbstractItemModel::setData(index, value, role);
}

Qt::ItemFlags GLDSchemaTreeModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags flag = QAbstractItemModel::flags(index);
    return flag;
}

bool GLDSchemaTreeModel::insertRows(int row, int count, const QModelIndex& parent)
{
    beginInsertRows(parent, row, row + count);

    GLDTreeNode* pParent = m_pDataSource;
    if (parent.isValid())
    {
        pParent = static_cast<GLDTreeNode*>(parent.internalPointer());
    }

    for (int i = 0; i < count; ++i)
    {
        GLDTreeNode* pNode = new GLDTreeNode(pParent);
        pNode->m_Icon      = m_nodeIcon;
        pNode->m_className = "test";
        pParent->insertNode(row, pNode);
    }
    endInsertRows();

    return true;
}

bool GLDSchemaTreeModel::removeRows(int row, int count, const QModelIndex& parent)
{
    beginRemoveRows(parent, row, row + count);
    GLDTreeNode* pParent = m_pDataSource;
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

void GLDSchemaTreeModel::initModel()
{
    m_pDataSource = new GLDTreeNode(nullptr);
    for (const auto& it : GLDExpPraseInfo::m_allClass)
    {
        int numeOfInst = 0;

        GLDTreeNode* pNode = new GLDTreeNode(m_pDataSource);
        pNode->m_className = QString::fromStdString(it.second->m_className);

        std::string name_  = it.second->m_className;
        numeOfInst = GLDExpPraseInfo::m_allInstanceNum.find(name_)->second;
        pNode->m_className += QString("(") + QString::fromStdString(std::to_string(numeOfInst)) + QString("/0)");

        m_pDataSource->addNode(pNode);
        pNode->m_Icon = m_nodeIcon;

        GLDTreeNode* pChildNode = new GLDTreeNode(pNode);
        pChildNode->m_className = QString("Attributes");
        pChildNode->m_Icon      = m_fileIcon;
        pNode->addNode(pChildNode);

        for (const auto& pro : it.second->m_attribute)
        {
            GLDTreeNode* pGrandChildNode = new GLDTreeNode(pChildNode);
            pGrandChildNode->m_className = QString::fromStdString(pro->m_attributeName);
            pGrandChildNode->m_Icon      = m_propIcon;
            pChildNode->addNode(pGrandChildNode);
        }
    }
}
