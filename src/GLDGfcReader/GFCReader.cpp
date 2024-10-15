#include "ExpressData.h"
#include "GFCReader.h"
#include <QFile>
#include <QRegularExpression>

GFCReader::GFCReader()
{
}

GFCReader::GFCReader(const QString& expPath, const QString& gfcPath)
{
    m_ExpressParser.Parse(expPath);
}


bool GFCReader::LoadExpressFile(const QString& expPath)
{
    m_ExpressParser.Parse(expPath);
    // m_ExpressParser.print();
    m_ExpData = m_ExpressParser.GetData();
    return true;
}

bool GFCReader::LoadGfcFile(const QString& gfcPath)
{
    QFile file(gfcPath);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "ERROR::Failed to open file!\n";
    }

    QTextStream in(&file);
    QString     line;
    while (in.readLineInto(&line))
    {
        if (line.contains("#") && line.contains("="))
        {
            ParseLine(line);
        }
    }

    return true;
}

void GFCReader::ParseLine(const QString& line)
{
    QRegularExpression      regex(R"(#(\d+)=([A-Z]+[A-Z0-9]*)\((.*)\);?)");
    QRegularExpressionMatch match = regex.match(line);
    if (match.hasMatch())
    {
        QString number     = match.captured(1);
        QString name       = match.captured(2);
        QString parameters = match.captured(3);

        if (m_ExpData.contains(name))
        {
            auto entityData = m_ExpData.value(name).dynamicCast<EntityData>();

            QVector<Attribute> attrs = GetAllAttributes(entityData);

            QStringList paraList = SplitParameters(parameters);

            for (int i = 0; i < attrs.size() && i < paraList.size(); ++i)
            {
                attrs[i].m_Value = paraList[i];
            }

            GFCEntity entity("#" + number, name, attrs);
            entityData->AppendEntity(entity);
        }
    }
}

QVector<Attribute> GFCReader::GetAllAttributes(QSharedPointer<EntityData> entityData)
{
    QVector<Attribute> attrs     = entityData->GetAttributes();
    QString            superType = entityData->GetSuperType().toUpper();

    while (!superType.isEmpty() && m_ExpData.contains(superType))
    {
        auto superEntityData = m_ExpData.value(superType).dynamicCast<EntityData>();
        attrs.append(superEntityData->GetAttributes());
        superType = superEntityData->GetSuperType().toUpper();
    }

    return attrs;
}

QStringList GFCReader::SplitParameters(const QString& parameters)
{
    QStringList result;
    int         parenthesesCount = 0;
    QString     currentParam;

    for (QChar c : parameters)
    {
        if (c == '(' || c == '[')
        {
            parenthesesCount++;
        }
        else if (c == ')' || c == ']')
        {
            parenthesesCount--;
        }

        if (c == ',' && parenthesesCount == 0)
        {
            result.append(currentParam.trimmed());
            currentParam.clear();
        }
        else
        {
            currentParam.append(c);
        }
    }

    if (!currentParam.isEmpty())
    {
        result.append(currentParam.trimmed());
    }

    return result;
}

void GFCReader::PrintGfc()
{
    for (auto& it : m_ExpData)
    {
        if (it->GetTypeName() == "ENTITY")
        {
            auto entityData = it.dynamicCast<EntityData>();
            auto entities   = entityData->GetEntities();

            qDebug() << entities.size();
            for (auto i : entities)
            {
                qDebug() << i.GetName() << i.GetNumber();
                for (auto attr : i.GetAttributes())
                {
                    qDebug() << attr.m_Value;
                }
            }
        }
    }
}

// Clear express data
void GFCReader::ClearEntityClass()
{
    m_ExpData.clear();
    m_ExpressParser.ClearAllClass();
}

// Add a entity by a string
void GFCReader::AddGfcEntity(const QString& line)
{
    ParseLine(line);
}

// Delete all entities by a class name (Upper word)
void GFCReader::DeleteGfcEntitiesByClassName(const QString& className)
{
    try
    {
        auto it = m_ExpData.find(className);
        if (it != m_ExpData.end())
        {
            it->reset();
        }
    }
    catch (...)
    {
        qDebug() << "Catch some exceptions.";
    }
}

// Delete a entity by number
void GFCReader::DeleteGfcEntityByNumberAndClassName(const int& number, const QString& className)
{
    try
    {
        auto it = m_ExpData.find(className);
        if (it != m_ExpData.end())
        {
            auto entities = it->dynamicCast<EntityData>()->GetEntities();
            for (auto iter = entities.begin(); iter != entities.end(); ++iter)
            {
                if (iter->GetNumber() == QString::number(number))
                {
                    entities.erase(iter);
                    break;
                }
            }
        }
    }
    catch (...)
    {
        qDebug() << "Catch some exceptions.";
    }
}

// Clear all gfc entities
void GFCReader::DeleteAllGfcEntity()
{
    for (auto& entityData : m_ExpData)
    {
        if (entityData->GetType() == Type::ENTITY)
            entityData.dynamicCast<EntityData>()->DeleteAllEntities();
    }
}

void GFCReader::PrintExp()
{
    m_ExpressParser.print();
}

GFCReader::~GFCReader()
{
}
