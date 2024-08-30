#pragma once
#include <vector>
#include <string>
#include <map>
#include <set>
#include <QString>

std::map<int, std::pair<std::string, std::string>> readGFCFile(const std::string& fileName);
std::map<int, std::pair<QString, QString>>         readGBKFile(const QString& filePath);
// 基础类，所有Exp解析出的数据继承自这个类
class GLDAbstract
{
  public:
    virtual std::string getName()
    {
        return std::string("null");
    };
};

// 基础数据类型，枚举类、四个基础类型的父类继承自该类
class GLDBasicDataType : public GLDAbstract
{
  public:
    std::string m_basicDataName;
    std::string getName()
    {
        return m_basicDataName;
    }
};

// 基础枚举类
class GLDBasicEnum : public GLDBasicDataType
{
  public:
    std::vector<std::string> m_enumName;
    int                      index{ 0 };
    std::string              getName()
    {
        return m_basicDataName;
    }
};

// 基础数据类，四个数据类型继承自该类
class GLDBasicBuild : public GLDBasicDataType
{
  public:
    std::string getName()
    {
        return m_basicDataName;
    }
};

// 布尔类型
class GLDBooleanType : public GLDBasicBuild
{
  public:
    bool        m_boolean{ false };
    std::string getName()
    {
        return m_basicDataName;
    }
};

// 整型
class GLDIntegerType : public GLDBasicBuild
{
  public:
    int         m_integer{ 0 };
    std::string getName()
    {
        return m_basicDataName;
    }
};

// 实数
class GLDRealType : public GLDBasicBuild
{
  public:
    double      m_real{ 0.0 };
    std::string getName()
    {
        return m_basicDataName;
    }
};

// 字符串
class GLDStringType : public GLDBasicBuild
{
  public:
    std::string m_string;
    std::string getName()
    {
        return m_basicDataName;
    }
};

class GLDleafAttributeSingle
{
  public:
    // 若是基础数据类型，则记录数据类型的指针
    std::vector<GLDBasicDataType*> m_attributeBasicType;
};

class GLDEntity;
class GLDNonleafAttributeSingle
{
  public:
    // 若是基础数据类型，则记录数据类型的指针
    std::vector<GLDEntity*> m_attributeNonBasicType;
};

// 每一个属性值，里面存着一个
class GLDAbstractAttribute : public GLDAbstract
{
  public:
    bool        m_option{ false };
    bool        m_repeat{ false };
    int         m_num{ 0 };
    std::string m_attributeName;
    std::string m_attributeType;

    bool                       m_isBasicAttribute{ false };
    GLDleafAttributeSingle*    m_leafAttributeSingle{ nullptr };
    GLDNonleafAttributeSingle* m_nonLeafAttributeSingle{ nullptr };


    GLDAbstractAttribute()
    {
    }
    GLDAbstractAttribute(std::string attributeName, int attributeNum)
    {
    }

    std::string getName()
    {
        return m_attributeType;
    }
    virtual std::string getAttributeName()
    {
        return m_attributeName;
    }
};


class GLDleafAttribute : public GLDAbstractAttribute
{
  public:
    // 若是基础数据类型，则记录数据类型的指针
    std::vector<GLDBasicDataType*> m_attributeBasicType;
};

class GLDEntity;
class GLDNonleafAttribute : public GLDAbstractAttribute
{
  public:
    // 若不是基础数据类型，记录GLDEntity的指针
    std::vector<GLDEntity*> m_attributeNonBasicType;
};

class GLDEntity : public GLDAbstract
{
  public:
    std::string                        m_className;
    std::string                        m_fatherClass;
    GLDEntity*                         m_fatherPointer{ nullptr };
    std::vector<GLDAbstractAttribute*> m_attribute;
    std::vector<int>                   m_attributeNum;
    bool                               isAllBasicNum;

    GLDEntity();
    GLDEntity(std::string entityName, std::vector<GLDAbstractAttribute*> attributeArr);

    static void parseExp(std::string expFilePath);

    std::string getName()
    {
        return m_className;
    }
};

class GLDExpPraseInfo
{
  public:
    // 存储所有类名
    static std::set<std::string> m_allClassName;
    // 存储所有基础类名
    static std::map<std::string, std::pair<std::string, bool>> m_allBasicTypeName;

    // 存储所有类的信息
    static std::vector<GLDEntity*> m_allClassInfo;

    // 存储着解析文件之后得到的各个类及其对应的个数
    static std::map<std::string, int> m_allInstanceNum;


    // 存储所有类名及对应的具体结构
    static std::map<std::string, GLDEntity*> m_allClass;
    static std::map<std::string, GLDEntity*> m_allClassUpperClassName;

    static std::vector<GLDEntity*> parseFileGetInstance(std::map<int, std::pair<QString, QString>> data);
};
