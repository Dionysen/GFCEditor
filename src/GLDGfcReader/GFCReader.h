#include "ExpressData.h"
#include "ExpressParser.h"
class GFCReader
{
  public:
    GFCReader();
    GFCReader(const QString& expPath, const QString& gfcPath);
    ~GFCReader();

    // Load File
    bool LoadExpressFile(const QString& expPath);
    bool LoadGfcFile(const QString& gfcPath);

    // Clear express data
    void ClearEntityClass();

    // Add a entity by a string
    void AddGfcEntity(const QString& line);

    // Delete all entities by a class name (Upper word)
    void DeleteGfcEntitiesByClassName(const QString& className);

    // Delete a entity by number
    void DeleteGfcEntityByNumberAndClassName(const int& number, const QString& className);

    // Clear all gfc entities
    void DeleteAllGfcEntity();

    // Parse
    void               ParseLine(const QString& line);
    QStringList        SplitParameters(const QString& parameters);
    QVector<Attribute> GetAllAttributes(QSharedPointer<EntityData> entityData);

    // Print
    void PrintExp();
    void PrintGfc();

  public:
    inline QHash<QString, QSharedPointer<Data>>& GetExpressData()
    {
        return m_ExpData;
    }

  private:
    ExpressParser m_ExpressParser;

    // Key 为全大写字母的名称
    QHash<QString, QSharedPointer<Data>> m_ExpData;
};