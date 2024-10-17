#include "GLDStyleManager.h"

#include <QFile>
#include <QIcon>
#include <QDebug>

QScopedPointer<GLDStyleManager> GLDStyleManager::_instance;

GLDStyleManager::GLDStyleManager()
    : m_Theme("dark")
{
}

QIcon GLDStyleManager::getIcon(const QString& iconName)
{
    return QIcon(QString(":/icon_%1/%2").arg(m_Theme).arg(iconName));
}

QString GLDStyleManager::getStyleSheet()
{
    QString path = QString(":/stylesheet_%1/style").arg(m_Theme);
    QFile file(path);
    if (file.open(QFile::ReadOnly))
    {
        return file.readAll();
    }
    qDebug() << "Failed to open style sheet file:" << file.errorString();
    return "";
}

void GLDStyleManager::setTheme(const QString& styleName)
{
    m_Theme = styleName;
    emit themeChanged();
}

GLDStyleManager::~GLDStyleManager()
{
}
