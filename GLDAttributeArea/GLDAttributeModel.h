#ifndef GLDATTRIBUTEMODEL_H
#define GLDATTRIBUTEMODEL_H
#include <QAbstractTableModel>


struct entity
{
    QString strName;
    QString strValue;
};

class GLDAttributeModel : public QAbstractTableModel
{
public:
    GLDAttributeModel(QObject* parent = 0);
    ~GLDAttributeModel();

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;

    //用外部数据设置m_pTableData
    void setProperties(const QMap<QString, QVariant>& properties);

private:
    QVector<entity*> m_pTableData;
};


#endif // GLDATTRIBUTEMODEL_H
