#include "qboxlayout.h"
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

// TODO：懒加载
class GLDEditorWidget : public QWidget
{
    Q_OBJECT
  public:
    GLDEditorWidget(QWidget* parent = nullptr);
    ~GLDEditorWidget();


    inline QPlainTextEdit* getTextEditor()
    {
        return p_textEditor;
    }

    // File operator
    void newFile();
    void openFile();
    void saveFile();
    void saveAs();

    // 根据文本pText查找，支持【区分大小写】和【全字匹配】
    void findAllText(QString pText, bool isCaseSensitive, bool isWordMatch);
    int  findText(QString pText, bool isCaseSensitive, bool isWordMatch);
    void findNextText();
    void findPreviousText();

    // 根据pText查找，替换成rText
    void replaceText(QString pText, QString rText);

    QVector<int> lineNumber;   // 所有匹配的文本所在的行号
    int          currentLine;  // lineNumber不为空时findnext所在的行(索引

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
    void readRecentFiles();
    void writeRecentFiles();

  private:
    QString currentFilePath;

    QVBoxLayout*    p_mainLayout;
    QPlainTextEdit* p_textEditor;


    QString m_statusBarTextCursor;
    QString m_statusBarTextFileInfo;

    // -----
    bool m_isModify;

    QQueue<QString> m_recentFiles;
    QString         appDataPath;
};
