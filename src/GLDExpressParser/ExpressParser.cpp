#include "ExpressParser.h"
#include "ExpressData.h"
#include <QFile>
#include <QSharedPointer>

void ExpressParser::Parse(const QString& filename)
{
    m_AllData.clear();
    QFile file(filename);
    // 检查文件是否打开成功
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Failed to open file!\n";
        return;
    }

    QTextStream in(&file);
    QString     line;
    // 逐行读取文件，将每一个块封装传给对应的解析函数
    while (in.readLineInto(&line))
    {
        QString block{ "" };
        if (line.contains("TYPE"))
        {
            // 获得每一个块，push到list中，用于解析
            do
            {
                block += line + "\n";
                in.readLineInto(&line);
            } while (!line.contains("END_TYPE"));
            m_BaseInfo.append(block);
        }
        else if (line.contains("ENTITY"))
        {
            // 获得每一个块
            do
            {
                block += line + "\n";
                in.readLineInto(&line);
            } while (!line.contains("END_ENTITY"));
            m_EntityInfo.append(block);
        }
    }
    ParseBaseType();
    ParseEntity();
}

void ExpressParser::print()
{
    for (auto it : m_AllData)
    {
        // qDebug() << it.GetName() << "\t";
        if (it->GetTypeName() == "ENTITY")
            it->print();
    }
}


void ExpressParser::ParseBaseType()
{
    // First，第一遍解析，将所有内置类型读取
    for (auto& block : m_BaseInfo)
    {
        QStringList info = block.split(" ");
        for (auto& it : info)
        {
            it.remove(";\n");
        }
        if (info[0] != QString("TYPE"))
        {
            qDebug() << "ERROR\n";
            return;
        }
        InsertByBaseDataName(info[1], info[3]);

        // 枚举类型没有递归定义，因此一次解析即可
        if (info[3] == "ENUMERATION")
            ParseEnumType(info);
    }
    // Second，第二遍解析，将所有递归类型解析，如果两次也不能完全解析，就要循环直到完全解析
    for (auto& block : m_BaseInfo)
    {
        QStringList info = block.split(" ");
        for (auto& it : info)
        {
            it.remove(";\n");
        }
        if (info[0] != QString("TYPE"))
        {
            qDebug() << "ERROR\n";
            return;
        }
        InsertByExistBaseData(info[1], info[3]);
    }
}


// 基本类型解析和插入
void ExpressParser::InsertByBaseDataName(QString name, QString typeName)
{
    if (typeName == "BOOLEAN")
    {
        QSharedPointer<Data> data = QSharedPointer<BoolBaseData>::create(name);
        m_AllData.insert(name, data);
    }
    else if (typeName == "STRING")
    {
        QSharedPointer<Data> data = QSharedPointer<StringBaseData>::create(name);
        m_AllData.insert(name, data);
    }
    else if (typeName == "REAL")
    {
        QSharedPointer<Data> data = QSharedPointer<RealBaseData>::create(name);
        m_AllData.insert(name, data);
    }
    else if (typeName == "INTEGER")
    {
        QSharedPointer<Data> data = QSharedPointer<IntBaseData>::create(name);
        m_AllData.insert(name, data);
    }
}

// 插入基础类型
void ExpressParser::InsertByExistBaseData(QString name, QString existTypeName)
{
    if (m_AllData.contains(existTypeName))
    {
        QString type = m_AllData[existTypeName]->GetTypeName();
        InsertByBaseDataName(name, type);
    }
}

// 解析枚举类型
void ExpressParser::ParseEnumType(const QStringList& info)
{
    QString name     = info[1];
    QString enumsStr = info[4];
    enumsStr.remove("(");
    enumsStr.remove(")");

    QStringList enums = enumsStr.split("\n\t");
    enums.pop_front();
    InsertByEnumDataName(name, enums);
}

// 枚举类型解析和插入
void ExpressParser::InsertByEnumDataName(QString name, QStringList enums)
{
    QSharedPointer<Data> data = QSharedPointer<EnumData>::create(name, enums);
    m_AllData.insert(name, data);
}

// 解析Entity分块解析（逐个）
void ExpressParser::ParseEntity()
{
    for (auto& entityInfo : m_EntityInfo)
    {
        entityInfo.remove("\t");
        // entityInfo.remove(";");

        QStringList info = entityInfo.split("\n");
        info.pop_back();
        // 将提取出的单个文本块单独拿出解析
        ParseEntityBlock(info);
    }
}

// 解析单个Entity文本块
void ExpressParser::ParseEntityBlock(const QStringList& block)
{
    // 从块List创建块字符串
    QString blockStr;
    for (auto& it : block)
    {
        blockStr += it;
    }

#if 0
    for (auto& it : block)
    {
        qDebug() << it << "\n";
    }
    qDebug() << "\n";
#endif

    // Name
    QString name = block[0];
    name.remove("ENTITY ");

    // Inherit
    QString     superType{ "" };
    QStringList subTypes{};
    bool        isAbstract{ false };

    if (blockStr.contains("ABSTRACT"))
    {
        isAbstract = true;
    }
    if (blockStr.contains("SUBTYPE OF"))
    {
        superType = blockStr.mid(blockStr.indexOf("SUBTYPE OF (") + 12, blockStr.indexOf(");") - blockStr.indexOf("SUBTYPE OF (") - 12);
    }
    // 为节省性能不创建临时字符串，但必须将SUPERTYPE放到最后，直接在原字符串上修改
    if (blockStr.contains("SUPERTYPE"))
    {
        blockStr = blockStr.mid(blockStr.indexOf("(ONEOF(") + 7, blockStr.indexOf("))") - blockStr.indexOf("(ONEOF(") - 7);
        subTypes = blockStr.split(",");
    }

    // Attribute
    QVector<Attribute> attributes;

    for (auto& it : block)
    {
        if (it.contains(":"))
        {
            QString attributeName = it.mid(0, it.indexOf(" :"));

            QString attributeType = it.mid(it.indexOf(": ") + 2, it.indexOf(";") - it.indexOf(": ") + 2);


            if (attributeType.contains("OPTIONAL"))
            {
                attributeType.remove("OPTIONAL ");
                attributeType.remove(";");

                attributes.append({ attributeName, attributeType, true, false });
            }
            else if (attributeType.contains("LIST [0:?] OF"))
            {
                attributeType.remove("LIST [0:?] OF ");
                attributeType.remove(";");
                attributes.append({ attributeName, attributeType, false, true });
            }
            else
            {
                attributeType.remove(";");
                attributes.append({ attributeName, attributeType, false, false });
            }
        }
    }

    QSharedPointer<Data> data = QSharedPointer<EntityData>::create(name, isAbstract, superType, subTypes, attributes);
    m_AllData.insert(name.toUpper(), data);
}


void ExpressParser::ClearAllClass()
{
    m_AllData.clear();
}