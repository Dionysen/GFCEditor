#include "GLDEditorWidget.h"
#include "qboxlayout.h"
#include "qfont.h"
#include "qmessagebox.h"
#include "qnamespace.h"
#include "qobject.h"
#include "qplaintextedit.h"
#include "qtextedit.h"
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QFuture>
#include <QDebug>
#include <QRegularExpression>
#include <QTextCursor>
#include <QTextCodec>
#include <QTextCodec>
// ==================================
// ========= Highlighter ============
// ==================================
Highlighter::Highlighter(QTextDocument* parent)
    : QSyntaxHighlighter(parent)
{

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    // 设置关键字高亮

    // 定义对象（如 #6）的格式
    QTextCharFormat objectFormat;
    objectFormat.setForeground(Qt::blue);
    objectFormat.setFontWeight(QFont::Bold);
    HighlightingRule rule;
    rule.pattern = QRegularExpression(R"(#\d+)");  // 匹配类似 #6 的对象
    rule.format  = objectFormat;
    highlightingRules.append(rule);


    // 定义类名（如 GFCFLOOR）的格式
    QTextCharFormat classFormat;
    classFormat.setForeground(Qt::darkMagenta);
    classFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression(R"(\bGFC[A-Z]+\b)");  // 匹配类名（以 GFC 开头的大写单词）
    rule.format  = classFormat;
    highlightingRules.append(rule);

    // 参数格式
    QTextCharFormat singleQuoteContentFormat;
    singleQuoteContentFormat.setForeground(Qt::darkGreen);
    QRegularExpression pattern(R"('.*?')");
    highlightingRules.append({ pattern, singleQuoteContentFormat });
}

void Highlighter::highlightBlock(const QString& text)
{
    for (const HighlightingRule& rule : highlightingRules)
    {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext())
        {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}

// ==================================
// ========= EditorWidget ============
// ==================================

GLDEditorWidget::GLDEditorWidget(QWidget* parent)
    : QWidget(parent)
    , currentFilePath("untitled")
    , m_statusBarTextCursor("")
    , m_statusBarTextFileInfo("")
    , m_isModify(false)
{
    p_mainLayout = new QVBoxLayout;

    p_textEditor = new QPlainTextEdit(this);
    new Highlighter(this->p_textEditor->document());
    QFont font;
    font.setPointSize(13);
    p_textEditor->setFont(font);
    p_mainLayout->addWidget(p_textEditor);

    lineNumber.clear();
    currentLine = -1;

    p_mainLayout->setMargin(0);
    setLayout(p_mainLayout);

    connect(this->p_textEditor, &QPlainTextEdit::cursorPositionChanged, this, [this]() { updateCursorPosition(); });

    connect(this->p_textEditor, &QPlainTextEdit::textChanged, this, [this]() {
        m_isModify = true;
        emit signalUpdateFileName(currentFilePath, m_isModify);
    });

    m_recentFiles.clear();
}

void GLDEditorWidget::newFile()
{
    if (!handleCloseFile())
        return;

    currentFilePath = "untitled";
    this->p_textEditor->clear();
    this->p_textEditor->appendPlainText(
        QStringLiteral("HEADER;\nFILE_DESCRIPTION(('GFC3X2'),'65001');\nFILE_NAME('D:/gmep-platform/tests/GFC3_Export/导出GFC3_支持暖通P0P1导出/"
                       "暖通P0P1-origin.gfc','2023-04-07 "
                       "10:56:03',('zhangyy-v'),('Glodon'),'objectbuf','\','\');\nFILE_SCHEMA(('GFC3X2'));\nENDSEC;\nADS;\nDATA;\n"));

    QTextCursor cursor = p_textEditor->textCursor();
    cursor.movePosition(QTextCursor::End);
    p_textEditor->setTextCursor(cursor);
}

void GLDEditorWidget::openFile()
{
    if (!handleCloseFile())
        return;
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Open File", "", "Text Files (*.gfc);;All Files (*)");
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (file.open(QFile::ReadOnly | QFile::Text))
        {
            QTextStream in(&file);
            QTextCodec* codec(QTextCodec::codecForName("UTF-8"));
            in.setCodec(codec);
            p_textEditor->setPlainText(in.readAll());
            currentFilePath = fileName;
            file.close();


            //            // test find function
            //            QString pText("'Grid'");
            //            findText(pText,true,false);

            //            QString rText("'grid'");
            //            replaceText(pText, rText);


            updateFileInfo(currentFilePath);
            addRecentFiles();
            m_isModify = false;
        }
        else
        {
            QMessageBox::warning(nullptr, "Error", "Could not open file.");
        }
    }
}
void GLDEditorWidget::saveFile()
{
    if (currentFilePath.isEmpty())
    {
        saveAs();
    }
    else
    {
        QFile file(currentFilePath);
        if (!file.exists())
        {
            saveAs();
            return;
        }
        if (file.open(QFile::WriteOnly | QFile::Text))
        {
            QTextStream out(&file);
            QTextCodec* codec(QTextCodec::codecForName("UTF-8"));
            out.setCodec(codec);
            out << p_textEditor->toPlainText();
            file.close();
            m_isModify = false;
            emit signalUpdateFileName(currentFilePath, m_isModify);
            addRecentFiles();
        }
        else
        {
            QMessageBox::warning(nullptr, "Error", "Could not save file.");
        }
    }
}

void GLDEditorWidget::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Save File", "", "Text Files (*.gfc);;All Files (*)");
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (file.open(QFile::WriteOnly | QFile::Text))
        {
            QTextStream out(&file);
            QTextCodec* codec(QTextCodec::codecForName("UTF-8"));
            out.setCodec(codec);
            out << p_textEditor->toPlainText();
            currentFilePath = fileName;
            file.close();
            emit signalUpdateFileName(currentFilePath, m_isModify);
            addRecentFiles();
        }
        else
        {
            QMessageBox::warning(nullptr, "Error", "Could not save file.");
        }
    }
}


void GLDEditorWidget::findAllText(QString pText, bool isCaseSensitive, bool isWordMatch)
{
    lineNumber.clear();
    currentLine       = -1;
    QString     text  = p_textEditor->toPlainText();
    QStringList lines = text.split('\n');

    // 根据是否区分大小写构建正则表达式
    QRegularExpression::PatternOptions options = isCaseSensitive ? QRegularExpression::NoPatternOption : QRegularExpression::CaseInsensitiveOption;

    // 根据是否全字匹配构建正则表达式
    QString            patternStr = isWordMatch ? "\\b" + QRegularExpression::escape(pText) + "\\b" : QRegularExpression::escape(pText);
    QRegularExpression re(patternStr, options);

    for (int i = 0; i < lines.size(); ++i)
    {
        if (re.match(lines[i]).hasMatch())
        {
            lineNumber.append(i + 2);  // 行号从1开始计数
            qDebug() << i + 1 << '\n';
        }
    }

    qDebug() << lineNumber.size() << '\n';
}


int GLDEditorWidget::findText(QString pText, bool isCaseSensitive, bool isWordMatch)
{
    findAllText(pText, isCaseSensitive, isWordMatch);
    if (!lineNumber.isEmpty())
    {
        ++currentLine;
        return lineNumber.at(currentLine);
    }
    qDebug() << "没有符合的结果！" << '\n';
    return -1;
}

void GLDEditorWidget::findNextText()
{
    ++currentLine;
    if (currentLine == lineNumber.size())
    {
        qDebug() << "已查找到最后一个！" << '\n';
        currentLine = 0;
    }
    qDebug() << lineNumber.at(currentLine) << '\n';
    //    return lineNumber.at(currentLine);
}

void GLDEditorWidget::findPreviousText()
{
    --currentLine;
    if (currentLine < 0)
    {
        qDebug() << "已查找到第一个！" << '\n';
        currentLine = lineNumber.size() - 1;
    }
    qDebug() << lineNumber.at(currentLine);
    //    return lineNumber.at(currentLine);
}

void GLDEditorWidget::replaceText(QString pText, QString rText)
{
    if (currentLine < 0 || currentLine >= lineNumber.size())
    {
        qDebug() << "未查找到替换内容！" << '\n';
        return;
    }

    QString     text  = p_textEditor->toPlainText();
    QStringList lines = text.split('\n');

    int     num     = lineNumber.at(currentLine);
    QString oldLine = lines.at(num);
    QString newLine = oldLine.replace(pText, rText);
    lines[num]      = newLine;

    QString modifiedText;
    for (int i = 0; i < lines.size(); ++i)
    {
        modifiedText += lines.at(i);
        if (i < lines.size() - 1)
        {
            modifiedText += '\n';  // 除了最后一行外，每行后面都添加换行符
        }
    }
    p_textEditor->setPlainText(modifiedText);

    qDebug() << num << '\n';

    //    for (int i = 0; i < lines; i++) {
    //        QString line = lines.at(i);
    //        QString newLine = line.replace(pText, rText);
    //        lines[i] = newLine;
    //    }
}


void GLDEditorWidget::updateCursorPosition()
{
    QTextCursor cursor = p_textEditor->textCursor();

    int line   = cursor.blockNumber() + 1;
    int column = cursor.columnNumber() + 1;

    m_statusBarTextCursor = (QStringLiteral("行: %1, 列: %2").arg(line).arg(column));
    emit signalUpdateStatusBar(m_statusBarTextCursor, m_statusBarTextFileInfo);
}

void GLDEditorWidget::updateFileInfo(const QString& filePath)
{
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        p_textEditor->setPlainText(file.readAll());
        file.close();

        // 更新文件大小
        QFileInfo fileInfo(filePath);
        qint64    fileSize      = fileInfo.size();    // 文件大小（字节）
        double    fileSizeKB    = fileSize / 1024.0;  // 转换为 KB
        m_statusBarTextFileInfo = QStringLiteral("文件大小: %1 KB").arg(fileSizeKB, 0, 'f', 2);
        emit signalUpdateStatusBar(m_statusBarTextCursor, m_statusBarTextFileInfo);
    }
}


bool GLDEditorWidget::handleCloseFile()
{
    if (m_isModify)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(nullptr, QStringLiteral("文件未保存"), QStringLiteral("是否先保存文件？"),
                                      QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        if (reply == QMessageBox::Yes)
        {
            saveFile();
            return true;
        }
        else if (reply == QMessageBox::No)
        {
            return true;
        }
        else if (reply == QMessageBox::Cancel)
        {
            return false;
        }
    }
    return true;
}

void GLDEditorWidget::addRecentFiles()
{
    if (!m_recentFiles.contains(currentFilePath))
    {
        if (m_recentFiles.size() >= 5)
        {
            m_recentFiles.dequeue();
        }
        m_recentFiles.enqueue(currentFilePath);
        emit signalUpdateRecentFiles(m_recentFiles);
    }
}
