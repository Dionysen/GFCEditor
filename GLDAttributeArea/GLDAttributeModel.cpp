#include "GLDAttributeModel.h"

GLDAttributeModel::GLDAttributeModel(QObject *parent)
    :QAbstractTableModel(parent)
{

}

GLDAttributeModel::~GLDAttributeModel()
{
    //todo
}

int GLDAttributeModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_pTableData.count();
}

int GLDAttributeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant GLDAttributeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    switch (role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
    {
        int nRow = index.row();
        entity *ent = m_pTableData[nRow];
        if(index.column() == 0)
            return ent->strName;
        else if(index.column() == 1)
            return ent->strValue;
        break;
    }
    default:
        break;
    }
    return QVariant();
}
#include <QColor>
QVariant GLDAttributeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal)
    {
        switch (role)
        {
        case Qt::DisplayRole:
        {
            if (section == 0)
                return QStringLiteral("属性名称");
            else if (section == 1)
                return QStringLiteral("属性值");
            break;
        }
        case Qt::ForegroundRole:
        {
            if (section == 1)
               return QColor(Qt::red);
            break;
        }
        case Qt::BackgroundRole:
            if (section == 0)
                return QColor(Qt::red);
            break;
        default:
            break;
        }
    }
    return QVariant();
}
#include <QDebug>
bool GLDAttributeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
        return false;
    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
    {
        entity *ent = m_pTableData[index.row()];
        if (index.column() == 1)
            ent->strValue = value.toString();
        qDebug() << ent->strValue;
        break;
    }
    default:
        break;
    }
    emit dataChanged(index,index);
    return true;
}

Qt::ItemFlags GLDAttributeModel::flags(const QModelIndex &index) const
{
    if(index.column() == 1)
        return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
    return QAbstractTableModel::flags(index);
}


void GLDAttributeModel::setProperties(const QMap<QString, QVariant>& properties)
{
    for (auto it = properties.begin(); it != properties.end(); ++it)
    {
        entity *ent = new entity;
        ent->strName = it.key();
        ent->strValue = it.value().toString();
        m_pTableData.append(ent);
    }
}

