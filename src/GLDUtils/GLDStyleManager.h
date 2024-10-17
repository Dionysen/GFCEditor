#pragma once
#include <QObject>

class GLDStyleManager : public QObject
{
    Q_OBJECT
  public:
    static GLDStyleManager* StyleManager()
    {
        if (!_instance)
            _instance.reset(new GLDStyleManager());
        return _instance.data();
    }

    QIcon   getIcon(const QString& iconName);
    QString getStyleSheet();

    void setTheme(const QString& styleName);
    ~GLDStyleManager();
  signals:
    void themeChanged();

  private:
    static QScopedPointer<GLDStyleManager> _instance;

    GLDStyleManager();


    QString m_Theme;
};
