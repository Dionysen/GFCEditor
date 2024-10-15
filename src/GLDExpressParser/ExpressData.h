#pragma once
#include "GFCEntity.h"

#include <QString>
#include <QStringList>
#include <QDebug>
#include <QHash>

enum class Type
{
    BOOL = 0,
    REAL,
    STRING,
    DOUBLE,
    INTEGER,
    ENUMERATION,
    ENTITY
};

class Data
{
  public:
    Data(const QString& name, const Type& type, const QString& typeName)
        : m_Name(name)
        , m_Type(type)
        , m_TypeName(typeName)
    {
    }

    virtual ~Data()
    {
    }

    inline const QString GetName()
    {
        return m_Name;
    }
    inline void SetName(const QString& name)
    {
        m_Name = name;
    }

    inline const Type GetType()
    {
        return m_Type;
    }
    inline void SetType(const Type& type)
    {
        m_Type = type;
    }


    inline const QString GetTypeName()
    {
        return m_TypeName;
    }
    inline void SetTypeName(const QString& type)
    {
        m_TypeName = type;
    }

    virtual void print()
    {
        qDebug() << "Name: " << m_Name << "\t\t"
                 << "Data: " << m_TypeName;
    };

  protected:
    QString m_Name;
    Type    m_Type;
    QString m_TypeName;
};

class BaseData : public Data
{
  public:
    BaseData(const QString& name, const Type& type, const QString& typeName)
        : Data(name, type, typeName)
    {
    }
};

class BoolBaseData : public BaseData
{
  public:
    BoolBaseData(const QString& name)
        : BaseData(name, Type::BOOL, "BOOLEAN")
    {
    }


  private:
    bool m_Data;
};
class StringBaseData : public BaseData
{
  public:
    StringBaseData(const QString& name)
        : BaseData(name, Type::STRING, "STRING")
    {
    }


  private:
    QString m_Data;
};

class RealBaseData : public BaseData
{
  public:
    RealBaseData(const QString& name)
        : BaseData(name, Type::REAL, "REAL")
    {
    }

  private:
    double m_Data;
};

class IntBaseData : public BaseData
{
  public:
    IntBaseData(const QString& name)
        : BaseData(name, Type::INTEGER, "INTEGER")
    {
    }

  private:
    int m_Data;
};



class EnumData : public Data
{
  public:
    EnumData(const QString& name, const QStringList& enums)
        : Data(name, Type::ENUMERATION, "ENUMERATION")
    {
        m_Enums = enums;
    }

    virtual void print() override
    {
        qDebug() << "Name: " << m_Name << "\t"
                 << "Data: " << m_Enums;
    }

  private:
    QStringList m_Enums;
};


class EntityData : public Data
{
  public:
    EntityData(const QString& name)
        : Data(name, Type::ENTITY, "ENTITY")
    {
        m_Entities.clear();
    }
    EntityData(const QString& name, bool isAbstract, const QString& superType, const QStringList& subTypes, const QVector<Attribute>& attributes)
        : Data(name, Type::ENTITY, "ENTITY")
        , m_IsAbstract(isAbstract)
        , m_SuperType(superType)
        , m_SubTypes(subTypes)
        , m_Attributes(attributes)
    {
        m_Entities.clear();
    }

    void print() override
    {
        qDebug() << "Name:" << m_Name << "Data:" << m_TypeName;
        if (m_IsAbstract)
            qDebug() << "ABSTRACT";
        qDebug() << "SuperType:" << m_SuperType;
        qDebug() << "SubTypes:" << m_SubTypes;

        for (auto it = m_Attributes.begin(); it != m_Attributes.end(); ++it)
        {
            if (it->m_IsList)
                qDebug() << it->m_TypeName << "(" << it->m_Name << ")" << ":" << it->m_Value << "LIST";
            else if (it->m_IsOptional)
                qDebug() << it->m_TypeName << "(" << it->m_Name << ")" << ":" << it->m_Value << "OPTIONAL";
            else
                qDebug() << it->m_TypeName << "(" << it->m_Name << ")" << ":" << it->m_Value;
        }
        qDebug() << "";
    }

    bool GetIsAbstract()
    {
        return m_IsAbstract;
    }
    QString GetSuperType()
    {
        return m_SuperType;
    }

    QStringList GetSubTypes()
    {
        return m_SubTypes;
    }
    QVector<Attribute>& GetAttributes()
    {
        return m_Attributes;
    }

    QVector<GFCEntity> GetEntities()
    {
        return m_Entities;
    }

    void DeleteAllEntities()
    {
        m_Entities.clear();
    }

    void AppendEntity(GFCEntity& entity)
    {
        m_Entities.append(entity);
    }

  private:
    bool               m_IsAbstract;
    QString            m_SuperType;
    QStringList        m_SubTypes;
    QVector<Attribute> m_Attributes;

    QVector<GFCEntity> m_Entities;
};
