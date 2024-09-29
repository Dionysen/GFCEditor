#ifndef GLDEditorWidget_H
#define GLDEditorWidget_H
#pragma once
#include "qchar.h"
#include "qcontainerfwd.h"
#include "qobject.h"
#include "qqueue.h"
#include "qwidget.h"
#include <QFileInfo>
#include <QPlainTextEdit>
#include <QQueue>
#include <QTextCharFormat>
#include <QTextEdit>
#include "GLDHighlighter.h"
#include <QRubberBand>
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
    void findAllText(QString pText, bool isCaseSensitive, bool isWordMatch);

    // 返回下一个匹配到的行号
    int findNextText();

    // 返回上一个匹配到的行号
    int findPreviousText();

    // 高亮当前匹配的行
    void highlightCurrentMatch(int lineNum);
    void clearAllHighlights();

    // 根据pText查找，替换成rText
    void replaceText(QString pText, QString rText);

    // 返回匹配到的所有行文本及对应行号
    QMap<int, QString> getMatchedList();
    int                getCurrentMatchNumber();

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
    QVector<int> lineNumber;   // 所有匹配的文本所在的行号
    int          currentLine;  // lineNumber不为空时findnext所在的行(索引

    // 当前查找的文本
    QString m_currentFindText;
    bool    m_isCaseSensitive;
    bool    m_isWordMatch;
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
