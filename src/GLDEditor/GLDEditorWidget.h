#ifndef GLDEditorWidget_H
#define GLDEditorWidget_H
#pragma once
#include "GLDHighlighter.h"

#include <QChar>
#include <QQueue>
#include <QFileInfo>
#include <QPlainTextEdit>
#include <QQueue>
#include <QTextCharFormat>
#include <QTextEdit>
// TODO：懒加载
class GLDEditorWidget : public QPlainTextEdit
{
    Q_OBJECT
  public:
    GLDEditorWidget(QWidget* parent = nullptr);
    ~GLDEditorWidget();

    // File operator
    void newFile();
    void openFile();
    void saveFile();
    void saveAs();

    // 根据文本pText查找，支持【区分大小写】和【全字匹配】
    QPair<int, int> findAllText(QString pText, bool isCaseSensitive, bool isWordMatch);
    bool            isNewSearch()
    {
        return m_isNewSearch;
    }
    // 返回下一个匹配到的行号
    QPair<int, int> findNextText();

    // 返回上一个匹配到的行号
    QPair<int, int> findPreviousText();

    // 高亮当前匹配的行
    void highlightCurrentMatch(QPair<int, int> matchIndex);
    void clearAllHighlights();

    // 根据pText查找，替换成rText
    void replaceText(QString pText, QString rText);

    // 返回匹配到的所有行文本及对应行号
    QMap<QPair<int, int>, QString> getMatchedList();
    int                            getCurrentMatchNumber();

    bool setEditorFontFamily(const QString& fontFamily);
    bool setFontSize(int size);

  signals:
    bool signalUpdateStatusBar(const QString& cursor, const QString& fileInfo);
    bool signalUpdateFileName(const QString& filename, bool isModify);
    bool signalUpdateRecentFiles(QQueue<QString>& recentFiles);


    // =========== Private ===========
  private slots:
    bool handleCloseFile();
    void updateCursorPosition();
    void updateFileInfo(const QString& filePath);

  private:
    void addRecentFiles();
    void readRecentFiles();
    void writeRecentFiles();

  private:
    QVector<QPair<int, int>> lineNumber;         // 所有匹配的文本所在的行号与索引
    int                      currentMatchIndex;  // lineNumber不为空时findnext所在的行(索引

    // 当前查找的文本
    QString m_currentFindText;
    bool    m_isCaseSensitive;
    bool    m_isWordMatch;
    bool    m_isNewSearch;
    // -------------------

    QString currentFilePath;

    QString m_statusBarTextCursor;
    QString m_statusBarTextFileInfo;

    // -----
    bool m_isModify;

    QQueue<QString> m_recentFiles;
    QString         appDataPath;
};

#endif  // GLDEditorWidget_H
