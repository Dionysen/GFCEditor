#include "qboxlayout.h"
#include "qchar.h"
#include "qcontainerfwd.h"
#include "qlist.h"
#include <QFileInfo>
#include "qobject.h"
#include "qqueue.h"
#include "qtextedit.h"
#include "qwidget.h"
#include <QTextEdit>
#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QTextCharFormat>
#include <QPlainTextEdit>
#include <QQueue>

class Highlighter : public QSyntaxHighlighter
{
  public:
    Highlighter(QTextDocument* parent = nullptr);

  protected:
    void highlightBlock(const QString& text) override;

  private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat    format;
    };
    QVector<HighlightingRule> highlightingRules;
};


// TODO：懒加载
class GLDEditorWidget : public QWidget
{
    Q_OBJECT
  public:
    GLDEditorWidget(QWidget* parent = nullptr);

    QPlainTextEdit* getTextEditor()
    {
        return p_textEditor;
    }

    void openFile();
    void saveFile();
    void saveAs();
    void newFile();

  signals:
    bool signalUpdateStatusBar(const QString& cursor, const QString& fileInfo);
    bool signalUpdateFileName(const QString& filename, bool isModify);
    bool signalUpdateRecentFiles(QQueue<QString>& recentFiles);

  private slots:

    bool handleCloseFile();
    void updateCursorPosition();
    void updateFileInfo(const QString& filePath);

  private:
    void addRecentFiles();

  public:
    // 根据文本pText查找，支持【区分大小写】和【全字匹配】
    void findAllText(QString pText, bool isCaseSensitive, bool isWordMatch);
    int  findText(QString pText, bool isCaseSensitive, bool isWordMatch);
    void findNextText();
    void findPreviousText();

    // 根据pText查找，替换成rText
    void replaceText(QString pText, QString rText);


    QVector<int> lineNumber;   // 所有匹配的文本所在的行号
    int          currentLine;  // lineNumber不为空时findnext所在的行(索引

  private:
    QString currentFilePath;

    QVBoxLayout*    p_mainLayout;
    QPlainTextEdit* p_textEditor;



    bool m_isModify;

    QString m_statusBarTextCursor;
    QString m_statusBarTextFileInfo;

    QQueue<QString> m_recentFiles;
};
