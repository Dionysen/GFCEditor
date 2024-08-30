#include "GLDEntity.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <windows.h>
#include <cctype>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>
#include <QChar>

// 初始化静态成员变量
std::set<std::string>                               GLDExpPraseInfo::m_allClassName;
std::map<std::string, std::pair<std::string, bool>> GLDExpPraseInfo::m_allBasicTypeName;
std::vector<GLDEntity*>                             GLDExpPraseInfo::m_allClassInfo;
std::map<std::string, GLDEntity*>                   GLDExpPraseInfo::m_allClass;
std::map<std::string, GLDEntity*>                   GLDExpPraseInfo::m_allClassUpperClassName;
std::map<std::string, int>                          GLDExpPraseInfo::m_allInstanceNum;

// 将字符串使用空格隔开
std::vector<std::string> splitStringBySpace(const std::string& str)
{
    std::vector<std::string> tokens;
    std::istringstream       tokenStream(str);
    std::string              token;

    while (std::getline(tokenStream, token, ' '))
    {
        // 注意：如果字符串中有连续的空格，上面的循环会跳过空字符串
        // 如果你想要保留这些空字符串（例如，作为分隔符的标记），
        // 你需要稍微修改逻辑来显式地添加它们。
        if (!token.empty())
        {  // 可选：如果你不想包含空字符串
            tokens.push_back(token);
        }
        // 或者，如果你想要保留所有分隔符（包括连续的空格作为空字符串）
        // 你可以直接添加token，不需要检查是否为空
        // tokens.push_back(token);
    }

    return tokens;
}

// 将字符使用特定字符拆开
std::vector<std::string> splitString(const std::string& s, char delimiter = ',')
{
    std::vector<std::string> tokens;
    std::string              token;
    std::istringstream       tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

QStringList splitString(const QString& input, char delimiter = ',')
{
    // 将char转换为QString，以便可以使用QString::split()方法
    QString separatorStr(delimiter);
    // 使用分隔符拆分QString
    return input.split(separatorStr);
}

// 确定是否一个字符串中是否存在另一个字符串
bool isContainStr(const std::string& source, const std::string& target)
{
    return source.find(target) != std::string::npos;
}

// 将（）中的字符拆分出来,读取一个类时拆分出其父类
std::string extractContentBetweenParentheses(const std::string& str)
{
    std::string result;
    bool        foundOpenParen = false;

    for (char c : str)
    {
        if (c == '(')
        {
            // 找到左括号，开始提取内容
            foundOpenParen = true;
            continue;  // 跳过左括号本身，不加入结果
        }
        else if (c == ')')
        {
            // 找到右括号，结束提取
            if (foundOpenParen)
            {
                break;  // 退出循环
            }
        }
        else if (foundOpenParen)
        {
            // 如果在左括号和右括号之间，则加入结果
            result += c;
        }
    }

    return result;
}

// 判断是否为基础数据类型
bool isBasicDataType(std::string checkStr, std::vector<GLDBasicDataType*> basicDataArr)
{
    for (int i = 0; i < basicDataArr.size(); i++)
    {
        if (checkStr == basicDataArr[i]->getName() || checkStr == "INTEGER" || checkStr == "STRING" || checkStr == "REAL" ||
            checkStr == "ENUMERATION")
        {
            return true;
        }
    }
    return false;
}

// 拆分gfc文件中的每一个实例的属性
std::vector<std::string> splitStringWithBrackets(const std::string& s)
{
    std::vector<std::string> result;
    std::string              current;
    bool                     inBrackets = false;

    for (char c : s)
    {
        if (c == '(')
        {
            inBrackets = true;
            if (!current.empty())
            {
                // 如果之前累积了内容，且现在进入括号，则先保存这部分内容
                result.push_back(current);
                current.clear();
            }
            // 括号内容单独处理
            current += c;
        }
        else if (c == ')' && inBrackets)
        {
            inBrackets = false;
            // 括号结束，直接添加到结果中
            result.push_back(current);
            current.clear();
        }
        else if (c == ',' && !inBrackets)
        {
            // 不在括号内且遇到逗号，保存当前累积的内容
            if (!current.empty())
            {
                result.push_back(current);
                current.clear();
            }
            // 逗号本身不加入结果
        }
        else
        {
            // 普通字符或括号内字符，直接累加到current
            current += c;
        }
    }

    // 循环结束后，如果current非空，则添加到最后
    if (!current.empty())
    {
        result.push_back(current);
    }

    return result;
}

// 拆分gfc文件中的每一个实例的属性
QStringList splitStringWithBrackets(const QString& input)
{
    QStringList result;
    QString     currentItem;
    bool        inParentheses = false;

    // 遍历输入字符串的每个字符
    for (QChar ch : input)
    {
        if (ch == '(')
        {
            // 如果遇到左括号，并且当前项不为空，则将其添加到结果列表中
            // 并标记为在括号内
            if (!currentItem.isEmpty())
            {
                result.append(currentItem.trimmed());  // 可能需要去除前后的空格或特殊字符
                currentItem.clear();
            }
            inParentheses = true;
            currentItem.append(ch);  // 将左括号也加入当前项
        }
        else if (ch == ')')
        {
            // 如果遇到右括号，则标记为不在括号内，并将当前项添加到结果列表中
            inParentheses = false;
            currentItem.append(ch);                // 将右括号也加入当前项
            result.append(currentItem.trimmed());  // 去除可能的前后空格
            currentItem.clear();
        }
        else if (!inParentheses && ch == ',')
        {
            // 如果不在括号内且遇到逗号，则将当前项添加到结果列表中，并清空当前项
            if (!currentItem.isEmpty())
            {
                result.append(currentItem.trimmed());
                currentItem.clear();
            }
            // 逗号本身不加入任何项
        }
        else
        {
            // 其他情况，将字符添加到当前项
            currentItem.append(ch);
        }
    }

    // 处理字符串末尾的情况（如果最后一个元素不是空且不在括号内）
    if (!currentItem.isEmpty() && !inParentheses)
    {
        result.append(currentItem.trimmed());
    }

    return result;
}

std::string toUpperCase(std::string& str)
{
    for (char& c : str)
    {
        c = std::toupper(static_cast<unsigned char>(c));  // 注意类型转换
    }
    return str;
}

void setEntityFlag()
{
    for (auto iter = GLDExpPraseInfo::m_allClass.begin(); iter != GLDExpPraseInfo::m_allClass.end(); iter++)
    {
        GLDEntity* temp = iter->second;
        for (int j = 0; j < temp->m_attribute.size(); j++)
        {
            if (!temp->m_attribute[j]->m_isBasicAttribute)
            {
                temp->m_attributeNum.push_back(0);
                temp->isAllBasicNum = false;
            }
            else
            {
                temp->m_attributeNum.push_back(1);
            }
        }
    }
}

int setEntity(GLDEntity*& data)
{
    if (data->isAllBasicNum || data->m_fatherClass == "Abstract Class")
    {
        return data->m_attribute.size();
    }
    for (int i = 0; i < data->m_attribute.size(); i++)
    {
        if (!data->m_attribute[i]->m_isBasicAttribute)
        {
            std::map<std::string, GLDEntity*>::iterator iter = GLDExpPraseInfo::m_allClass.find(data->m_attribute[i]->m_attributeType);
            data->m_attributeNum[i]                          = data->m_attributeNum[i] + setEntity(iter->second);
        }
    }
}



// 解析EXP
void GLDEntity::parseExp(std::string expFilePath)
{
    std::vector<GLDBasicDataType*> basicDataArr;
    std::fstream                   file(expFilePath);

    // 检查文件是否成功打开
    if (!file.is_open())
    {
        std::cerr << "Failed to open file" << std::endl;
        return;
    }

    std::string line;
    int         count = 0;

    while (std::getline(file, line))
    {
        // 处理TYPE
        if (line.substr(0, 4) == "TYPE")
        {
            std::vector<std::string> info = splitStringBySpace(line);
            // GLDBasicDataType
            if (info[info.size() - 1] == "BOOLEAN;")
            {
                GLDBooleanType* booleanTemp  = new GLDBooleanType;
                booleanTemp->m_basicDataName = info[1];
                booleanTemp->m_boolean       = false;

                basicDataArr.push_back(static_cast<GLDBasicDataType*>(booleanTemp));

                // 记录所有的基础数据类型
                auto iter = GLDExpPraseInfo::m_allBasicTypeName.find(info[1]);
                if (iter == GLDExpPraseInfo::m_allBasicTypeName.end())
                {
                    GLDExpPraseInfo::m_allBasicTypeName.insert(std::make_pair(info[1], std::make_pair("BOOLEAN", true)));
                }
            }
            else if (info[info.size() - 1] == "INTEGER;")
            {
                GLDIntegerType* integerTemp  = new GLDIntegerType;
                integerTemp->m_basicDataName = info[1];
                integerTemp->m_integer       = 0;

                basicDataArr.push_back(static_cast<GLDBasicDataType*>(integerTemp));
                // 记录所有的基础数据类型
                auto iter = GLDExpPraseInfo::m_allBasicTypeName.find(info[1]);
                if (iter == GLDExpPraseInfo::m_allBasicTypeName.end())
                {
                    GLDExpPraseInfo::m_allBasicTypeName.insert(std::make_pair(info[1], std::make_pair("INTEGER", true)));
                }
            }
            else if (info[info.size() - 1] == "REAL;")
            {
                GLDRealType* realTemp = new GLDRealType;

                realTemp->m_basicDataName = info[1];
                realTemp->m_real          = 0.0;
                basicDataArr.push_back(static_cast<GLDBasicDataType*>(realTemp));

                // 记录所有的基础数据类型
                auto iter = GLDExpPraseInfo::m_allBasicTypeName.find(info[1]);
                if (iter == GLDExpPraseInfo::m_allBasicTypeName.end())
                {
                    GLDExpPraseInfo::m_allBasicTypeName.insert(std::make_pair(info[1], std::make_pair("REAL", true)));
                }
            }
            else if (info[info.size() - 1] == "STRING;")
            {
                GLDStringType* stringTemp = new GLDStringType;

                stringTemp->m_basicDataName = info[1];
                stringTemp->m_string        = "";
                basicDataArr.push_back(static_cast<GLDBasicDataType*>(stringTemp));
                // 记录所有的基础数据类型
                auto iter = GLDExpPraseInfo::m_allBasicTypeName.find(info[1]);
                if (iter == GLDExpPraseInfo::m_allBasicTypeName.end())
                {
                    GLDExpPraseInfo::m_allBasicTypeName.insert(std::make_pair(info[1], std::make_pair("STRING", true)));
                }
            }
            else if (info[info.size() - 1] == "OF")
            {
                // 处理枚举类
                GLDBasicEnum*            enumTemp = new GLDBasicEnum;
                std::vector<std::string> info     = splitStringBySpace(line);

                enumTemp->m_basicDataName = info[1];
                while (std::getline(file, line) && line != "END_TYPE;")
                {
                    if (line[line.size() - 1] != ';')
                    {
                        enumTemp->m_enumName.push_back(line.substr(2, line.size() - 2));
                    }
                    else
                    {
                        enumTemp->m_enumName.push_back(line.substr(2, line.size() - 4));
                    }
                }
                enumTemp->index = 0;
                basicDataArr.push_back(static_cast<GLDBasicDataType*>(enumTemp));
                // 记录所有的基础数据类型
                auto iter = GLDExpPraseInfo::m_allBasicTypeName.find(info[1]);
                if (iter == GLDExpPraseInfo::m_allBasicTypeName.end())
                {
                    GLDExpPraseInfo::m_allBasicTypeName.insert(std::make_pair(info[1], std::make_pair("ENUMERATION", true)));
                }
            }
            else
            {
                GLDStringType* stringTemp = new GLDStringType;

                stringTemp->m_basicDataName = info[1];
                stringTemp->m_string        = "";
                basicDataArr.push_back(static_cast<GLDBasicDataType*>(stringTemp));

                // 记录所有的基础数据类型
                auto iter = GLDExpPraseInfo::m_allBasicTypeName.find(info[1]);
                if (iter == GLDExpPraseInfo::m_allBasicTypeName.end())
                {
                    GLDExpPraseInfo::m_allBasicTypeName.insert(
                        std::make_pair(info[1], std::make_pair(info[info.size() - 1].substr(0, info[info.size() - 1].size() - 1), false)));
                }
            }
        }

        // 处理ENTITY
        if (line.substr(0, 6) == "ENTITY")
        {
            std::vector<std::string> info = splitStringBySpace(line);

            GLDEntity* entityTemp     = new GLDEntity;
            entityTemp->m_className   = info[1];
            entityTemp->m_fatherClass = "Abstract Class";

            GLDExpPraseInfo::m_allClassName.insert(info[1]);
            GLDExpPraseInfo::m_allClass.insert(std::make_pair(info[1], entityTemp));
            GLDExpPraseInfo::m_allInstanceNum.insert(std::make_pair(info[1], 0));
            GLDExpPraseInfo::m_allClassUpperClassName.insert(std::make_pair(toUpperCase(info[1]), entityTemp));

            while (std::getline(file, line) && line != "END_ENTITY;")
            {
                if (isContainStr(line, ":"))
                {
                    // 如果是一个属性
                    std::vector<std::string> infoTemp = splitStringBySpace(line);
                    infoTemp[0]                       = infoTemp[0].substr(1, infoTemp[0].size() - 1);
                    infoTemp[infoTemp.size() - 1]     = infoTemp[infoTemp.size() - 1].substr(0, infoTemp[infoTemp.size() - 1].size() - 1);

                    // 设置类型名
                    if (isBasicDataType(infoTemp[infoTemp.size() - 1], basicDataArr))
                    {
                        GLDleafAttribute* tempAstractAttribute   = new GLDleafAttribute;
                        tempAstractAttribute->m_attributeType    = infoTemp[infoTemp.size() - 1];
                        tempAstractAttribute->m_attributeName    = infoTemp[0];
                        tempAstractAttribute->m_isBasicAttribute = true;
                        if (isContainStr(line, "OPTIONAL"))
                        {
                            tempAstractAttribute->m_option = true;
                        }
                        else
                        {
                            tempAstractAttribute->m_option = false;
                        }

                        if (isContainStr(line, "LIST"))
                        {
                            tempAstractAttribute->m_repeat = true;
                        }
                        else
                        {
                            tempAstractAttribute->m_repeat = false;
                        }

                        entityTemp->m_attribute.push_back(tempAstractAttribute);
                    }
                    else
                    {
                        GLDNonleafAttribute* tempAstractAttribute = new GLDNonleafAttribute;
                        tempAstractAttribute->m_attributeType     = infoTemp[infoTemp.size() - 1];
                        tempAstractAttribute->m_attributeName     = infoTemp[0];
                        // m_isBasicAttribute
                        tempAstractAttribute->m_isBasicAttribute = false;
                        if (isContainStr(line, "OPTIONAL"))
                        {
                            tempAstractAttribute->m_option = true;
                        }
                        else
                        {
                            tempAstractAttribute->m_option = false;
                        }

                        if (isContainStr(line, "LIST"))
                        {
                            tempAstractAttribute->m_repeat = true;
                        }
                        else
                        {
                            tempAstractAttribute->m_repeat = false;
                        }
                        entityTemp->m_attribute.push_back(tempAstractAttribute);
                    }
                }
                else
                {
                    // 不是属性则存储着父类或者子类的信息
                    // 设置父类
                    if (isContainStr(line, "SUBTYPE"))
                    {
                        entityTemp->m_fatherClass = extractContentBetweenParentheses(line);
                    }
                    else
                    {
                        // 设置子类，暂时不操作
                        int a = 10;
                    }
                }
            }
            GLDExpPraseInfo::m_allClassInfo.push_back(entityTemp);
        }
    }

    setEntityFlag();

    int count1 = 0;
    for (auto iter = GLDExpPraseInfo::m_allClass.begin(); iter != GLDExpPraseInfo::m_allClass.end(); iter++)
    {
        setEntity(iter->second);
        count1++;
        if (count1 == 107)
        {
            int a = 10;
        }
    }
    return;
}


GLDEntity* createEntity(QString entityName, QString data)
{
    // 根据类型名获取
    GLDEntity* temp = GLDExpPraseInfo::m_allClass.find(entityName.toStdString())->second;
    GLDEntity* res  = new GLDEntity(*temp);

    return res;
}


// 解析gfc得到数据
std::vector<GLDEntity*> GLDExpPraseInfo::parseFileGetInstance(std::map<int, std::pair<QString, QString>> data)
{
    std::vector<GLDEntity*> res;

    for (std::map<int, std::pair<QString, QString>>::iterator iter = data.begin(); iter != data.end(); iter++)
    {
        // 获取类名，在className中寻找这个实例化对象的实际类型，得到其具体信息
        GLDEntity* temp = GLDExpPraseInfo::m_allClassUpperClassName.find(iter->second.first.toStdString())->second;

        res.push_back(new GLDEntity(*temp));

        auto iterClassType = GLDExpPraseInfo::m_allInstanceNum.find(temp->m_className);
        if (iterClassType != GLDExpPraseInfo::m_allInstanceNum.end())
        {
            iterClassType->second++;
        }

        // 处理每一项数据,得到实例对象的属性
        QStringList singleInstanceInfo = splitStringWithBrackets(iter->second.second);

        int index = singleInstanceInfo.size() - 1;
        // 从后往前构造其内部属性
        for (int i = temp->m_attribute.size() - 1; i >= 0; i--)
        {
            GLDAbstractAttribute* singleAttribute = temp->m_attribute[i];

            // 如果是基础数据类型
            if (singleAttribute->m_isBasicAttribute)
            {
                // 若该属性是一个基础的数据类型，将其向下类型转换至GLDleafAttribute*
                GLDleafAttribute* singleAttributeDynamic = dynamic_cast<GLDleafAttribute*>(singleAttribute);
                singleAttribute->m_leafAttributeSingle   = new GLDleafAttributeSingle;

                // 在m_allBasicTypeName中查询该类的具体数据类型
                std::map<std::string, std::pair<std::string, bool>>::iterator iter =
                    GLDExpPraseInfo::m_allBasicTypeName.find(singleAttribute->m_attributeType);
                if (iter != GLDExpPraseInfo::m_allBasicTypeName.end())
                {
                    std::string realBasicType = iter->second.first;

                    if (realBasicType == "INTEGER")
                    {
                        // 是整数的基本数据类型
                        if (singleAttribute->m_repeat)
                        {
                            // 如果该数据是可重复的
                            // 解析该项类型的数据
                            QString str         = singleInstanceInfo[index];
                            str                 = str.mid(1, str.size() - 2);
                            QStringList strList = splitString(str, ',');

                            for (int i = 0; i < strList.size(); i++)
                            {
                                GLDIntegerType* t = new GLDIntegerType;
                                t->m_integer      = strList[i].toInt();
                                singleAttributeDynamic->m_attributeBasicType.push_back(t);
                                singleAttribute->m_leafAttributeSingle->m_attributeBasicType.push_back(t);
                            }
                        }
                        else if (singleAttribute->m_option)
                        {
                            // 如果该数据是可选的,若该对应数据记录的不为$
                            QString str = singleInstanceInfo[index];
                            if (str != "$")
                            {
                                GLDIntegerType* t = new GLDIntegerType;
                                t->m_integer      = str.toInt();
                                singleAttributeDynamic->m_attributeBasicType.push_back(t);
                                singleAttribute->m_leafAttributeSingle->m_attributeBasicType.push_back(t);
                            }
                        }
                        else
                        {
                            // 一般情况
                            // 如果该数据是可选的,若该对应数据不为$
                            QString str = singleInstanceInfo[index];

                            GLDIntegerType* t = new GLDIntegerType;
                            t->m_integer      = str.toInt();
                            singleAttributeDynamic->m_attributeBasicType.push_back(t);
                            singleAttribute->m_leafAttributeSingle->m_attributeBasicType.push_back(t);
                        }
                        index--;
                    }
                    else if (realBasicType == "REAL")
                    {
                        // 是实数的基本数据类型
                        if (singleAttribute->m_repeat)
                        {
                            // 如果该数据是可重复的
                            // 解析该项类型的数据
                            QString str         = singleInstanceInfo[index];
                            str                 = str.mid(1, str.size() - 2);
                            QStringList strList = splitString(str, ',');

                            for (int i = 0; i < strList.size(); i++)
                            {
                                GLDRealType* t = new GLDRealType;
                                t->m_real      = strList[i].toDouble();
                                singleAttributeDynamic->m_attributeBasicType.push_back(t);
                                singleAttribute->m_leafAttributeSingle->m_attributeBasicType.push_back(t);
                            }
                        }
                        else if (singleAttribute->m_option)
                        {
                            // 如果该数据是可选的,若该对应数据记录的不为$
                            QString str = singleInstanceInfo[index];
                            if (str != "$")
                            {
                                GLDRealType* t = new GLDRealType;
                                t->m_real      = str.toInt();
                                singleAttributeDynamic->m_attributeBasicType.push_back(t);
                                singleAttribute->m_leafAttributeSingle->m_attributeBasicType.push_back(t);
                            }
                        }
                        else
                        {
                            // 一般情况
                            // 如果该数据是可选的,若该对应数据不为$
                            QString str = singleInstanceInfo[index];

                            GLDRealType* t = new GLDRealType;
                            t->m_real      = str.toInt();
                            singleAttributeDynamic->m_attributeBasicType.push_back(t);
                            singleAttribute->m_leafAttributeSingle->m_attributeBasicType.push_back(t);
                        }
                        index--;
                    }
                    else if (realBasicType == "ENUMERATION")
                    {
                        // 是枚举的基本数据类型
                        index--;
                    }
                    else if (realBasicType == "BOOLEAN")
                    {
                        // 是布尔的基本数据类型
                        if (singleAttribute->m_repeat)
                        {
                            // 如果该数据是可重复的
                            // 解析该项类型的数据
                            QString str         = singleInstanceInfo[index];
                            str                 = str.mid(1, str.size() - 2);
                            QStringList strList = splitString(str, ',');

                            for (int i = 0; i < strList.size(); i++)
                            {
                                GLDBooleanType* t = new GLDBooleanType;
                                if (strList[i] == ".T.")
                                {
                                    t->m_boolean = false;
                                }
                                else
                                {
                                    t->m_boolean = true;
                                }
                                singleAttributeDynamic->m_attributeBasicType.push_back(t);
                                singleAttribute->m_leafAttributeSingle->m_attributeBasicType.push_back(t);
                            }
                        }
                        else if (singleAttribute->m_option)
                        {
                            // 如果该数据是可选的,若该对应数据记录的不为$
                            QString str = singleInstanceInfo[index];
                            if (str != "$")
                            {
                                GLDBooleanType* t = new GLDBooleanType;
                                if (str == ".T.")
                                {
                                    t->m_boolean = false;
                                }
                                else
                                {
                                    t->m_boolean = true;
                                }
                                singleAttributeDynamic->m_attributeBasicType.push_back(t);
                                singleAttribute->m_leafAttributeSingle->m_attributeBasicType.push_back(t);
                            }
                        }
                        else
                        {
                            // 一般情况
                            // 如果该数据是可选的,若该对应数据不为$
                            QString str = singleInstanceInfo[index];

                            GLDBooleanType* t = new GLDBooleanType;
                            if (str == ".T.")
                            {
                                t->m_boolean = false;
                            }
                            else
                            {
                                t->m_boolean = true;
                            }
                            singleAttributeDynamic->m_attributeBasicType.push_back(t);
                            singleAttribute->m_leafAttributeSingle->m_attributeBasicType.push_back(t);
                        }
                        index--;
                    }
                    else
                    {
                        // 是字符串的基本数据类型
                        if (singleAttribute->m_repeat)
                        {
                            // 如果该数据是可重复的
                            // 解析该项类型的数据
                            QString str         = singleInstanceInfo[index];
                            str                 = str.mid(1, str.size() - 2);
                            QStringList strList = splitString(str, ',');

                            for (int i = 0; i < strList.size(); i++)
                            {
                                GLDStringType* t = new GLDStringType;
                                QString        sss;
                                t->m_string = strList[i].mid(1, strList.size() - 2).toStdString();
                                singleAttributeDynamic->m_attributeBasicType.push_back(t);
                                singleAttribute->m_leafAttributeSingle->m_attributeBasicType.push_back(t);
                            }
                        }
                        else if (singleAttribute->m_option)
                        {
                            // 如果该数据是可选的,若该对应数据记录的不为$
                            QString str = singleInstanceInfo[index];
                            if (str != "$")
                            {
                                GLDStringType* t = new GLDStringType;
                                t->m_string      = str.mid(1, str.size() - 2).toStdString();
                                singleAttributeDynamic->m_attributeBasicType.push_back(t);
                                singleAttribute->m_leafAttributeSingle->m_attributeBasicType.push_back(t);
                            }
                        }
                        else
                        {
                            // 一般情况
                            // 如果该数据是可选的,若该对应数据不为$
                            QString str = singleInstanceInfo[index];

                            GLDStringType* t = new GLDStringType;
                            t->m_string      = str.mid(1, str.size() - 2).toStdString();
                            singleAttributeDynamic->m_attributeBasicType.push_back(t);
                            singleAttribute->m_leafAttributeSingle->m_attributeBasicType.push_back(t);
                        }
                        index--;
                    }
                }
            }
            else
            {
                // 若不是基础数据类型
                GLDNonleafAttribute* singleAttributeDynamic = dynamic_cast<GLDNonleafAttribute*>(singleAttribute);
                singleAttribute->m_nonLeafAttributeSingle   = new GLDNonleafAttributeSingle;

                if (singleInstanceInfo[index].indexOf(QChar('#')))
                {
                    // 若是一个引用
                    if (singleAttribute->m_repeat)
                    {
                        QString str         = singleInstanceInfo[index];
                        str                 = str.mid(1, str.size() - 2);
                        QStringList strList = splitString(str, ',');

                        for (int i = 0; i < strList.size(); i++)
                        {
                            int quoteIndex = strList[i].mid(1, strList[i].size() - 1).toInt();

                            if (quoteIndex <= res.size() && quoteIndex > 0)
                            {
                                singleAttributeDynamic->m_attributeNonBasicType.push_back(res[quoteIndex - 1]);
                            }
                        }
                    }
                    else if (singleAttribute->m_option)
                    {
                        QString str = singleInstanceInfo[index];
                        if (str != "$")
                        {
                            int quoteIndex = str.mid(1, str.size() - 1).toInt();

                            if (quoteIndex <= res.size() && quoteIndex > 0)
                            {
                                singleAttributeDynamic->m_attributeNonBasicType.push_back(res[quoteIndex - 1]);
                            }
                        }
                    }
                    else
                    {
                        QString str        = singleInstanceInfo[index];
                        int     quoteIndex = str.mid(1, str.size() - 1).toInt();

                        if (quoteIndex <= res.size() && quoteIndex > 0)
                        {
                            singleAttributeDynamic->m_attributeNonBasicType.push_back(res[quoteIndex - 1]);
                        }
                    }
                }
                else
                {
                    // 若不是引用
                }
                index--;
            }
        }
        // 如果有父类，剩余的数量给其父类构造其父类
        if (temp->m_fatherClass != "Abstract Class") {}
    }

    auto iii = GLDExpPraseInfo::m_allClassName.begin();
    for (std::map<std::string, int>::iterator iter = GLDExpPraseInfo::m_allInstanceNum.begin(); iter != GLDExpPraseInfo::m_allInstanceNum.end();
         iter++)
    {
        auto t  = GLDExpPraseInfo::m_allInstanceNum.find(*iii);
        auto tt = *t;
        iii++;
    }
    return res;
}
GLDEntity::GLDEntity()
{
}

std::map<int, std::pair<QString, QString>> readGBKFile(const QString& filePath)
{
    std::map<int, std::pair<QString, QString>> pEntity;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Unable to open file\n" << filePath;
        return pEntity;
    }

    // 设置QTextStream的编码为GBK
    QTextStream in(&file);
    in.setCodec("UTF-8");

    bool flag = false;
    // 逐行读取
    while (!in.atEnd())
    {
        QString pLine = in.readLine();
        // 处理每一行数据
        if (pLine == "DATA;")
        {
            flag = true;
            continue;
        }
        else if (pLine == "ENDSEC;")
        {
            flag = false;
        }

        if (flag == true)
        {
            std::size_t pos = pLine.indexOf('=');
            // 等号前#编号作为键值
            int pKey = pLine.mid(1, pos - 1).toInt();
            // std::cout << pKey << std::endl;   // test cout

            // 等号之后全部信息作为value{type, attribute}
            QString pValue = pLine.mid(pos + 1);

            // 读取括号里内容
            std::size_t posStart = pValue.indexOf('(');

            QString                     pType = pValue.mid(0, posStart);
            QString                     pItem = pValue.mid(posStart + 1, pValue.size() - posStart - 3);
            std::pair<QString, QString> mValue;
            mValue = std::make_pair(pType, pItem);

            // std::cout << pType << std::endl;
            // std::cout << pItem << std::endl;

            // 每一行信息添加到map
            pEntity.insert(std::make_pair(pKey, mValue));
        }
    }

    file.close();

    return pEntity;
}

std::map<int, std::pair<std::string, std::string>> readGFCFile(const std::string& fileName)
{
    SetConsoleOutputCP(CP_UTF8);

    std::map<int, std::pair<std::string, std::string>> pEntity;

    // 创建流对象
    std::ifstream pFile;
    // 打开文件
    pFile.open(fileName);

    // 错误提示
    if (!pFile.is_open())
    {
        std::cerr << "Unable to open file\n";
        return pEntity;
    }

    // 打开后读取信息
    std::string pLine;
    bool        flag = false;

    // 循环读取每一行
    while (getline(pFile, pLine))
    {
        // 处理每一行数据
        if (pLine == "DATA;")
        {
            flag = true;
            continue;
        }
        else if (pLine == "ENDSEC;")
        {
            flag = false;
        }

        if (flag == true)
        {
            std::size_t pos = pLine.find('=');
            // 等号前#编号作为键值
            int pKey = std::stoi(pLine.substr(1, pos));
            // std::cout << pKey << std::endl;   // test cout

            // 等号之后全部信息作为value{type, attribute}
            std::string pValue = pLine.substr(pos + 1);

            // 读取括号里内容
            std::size_t posStart = pValue.find('(');

            std::string                         pType = pValue.substr(0, posStart);
            std::string                         pItem = pValue.substr(posStart + 1, pValue.size() - posStart - 3);
            std::pair<std::string, std::string> mValue;
            mValue = std::make_pair(pType, pItem);

            // std::cout << pType << std::endl;
            // std::cout << pItem << std::endl;

            // 每一行信息添加到map
            pEntity.insert(std::make_pair(pKey, mValue));
        }
    }

    // 关闭文件
    pFile.close();
    return pEntity;
}
