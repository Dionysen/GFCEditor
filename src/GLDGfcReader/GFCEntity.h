#pragma once

#include <QString>
#include <QVector>

struct Attribute
{
    Attribute(const QString& name, const QString& typeName, bool isOptional, bool isList)
        : m_Name(name)
        , m_TypeName(typeName)
        , m_IsOptional(isOptional)
        , m_IsList(isList)
        , m_Value("")
    {
    }
    Attribute()
        : m_Name("")
        , m_TypeName("")
        , m_IsOptional(false)
        , m_IsList(false)
        , m_Value("")
    {
    }

    void print()
    {
    }
    QString m_Name;
    QString m_TypeName;
    QString m_Value;
    bool    m_IsOptional;
    bool    m_IsList;
};


/*
    GFC文件中，每一行都是一个实例对象，此对象通过Name与已解析的Exp文件中的类型相关联
    名称后面的括号内是构造参数
*/
class GFCEntity
{
  public:
    GFCEntity(const QString& number, const QString& name, const QVector<Attribute>& attr)
        : m_Number(number)
        , m_Name(name)
        , m_Attributes(attr)
    {
    }

    GFCEntity()
    {
    }
    const QString& GetName()
    {
        return m_Name;
    }
    const QString& GetNumber()
    {
        return m_Number;
    }

    const QVector<Attribute>& GetAttributes()
    {
        return m_Attributes;
    }

  private:
    // GFC文件中的编号
    QString m_Number;

    // 类名
    QString m_Name;

    // 参数，既可以是基础类型，也可以是类；
    // 参数的值使用String保存，需要时解析即可
    QVector<Attribute> m_Attributes;
};