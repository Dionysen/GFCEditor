#pragma once
#include "ExpressData.h"

class ExpressParser
{
  public:
    ExpressParser()
    {
    }
    ~ExpressParser() = default;

    void Parse(const QString& filename);

    // Parse Function
    void ParseBaseType();
    void ParseEntity();

    void ClearAllClass();


    // Tool funciton
    void InsertByBaseDataName(QString name, QString typeName);
    void InsertByExistBaseData(QString name, QString existTypeName);

    void ParseEnumType(const QStringList& info);
    void InsertByEnumDataName(QString name, QStringList enums);

    void ParseEntityBlock(const QStringList& block);

    // 打印
    void print();

    inline const QHash<QString, QSharedPointer<Data>>& GetData()
    {
        return m_AllData;
    }

  private:
    QStringList m_BaseInfo;
    QStringList m_EntityInfo;

    QHash<QString, QSharedPointer<Data>> m_AllData;
};
