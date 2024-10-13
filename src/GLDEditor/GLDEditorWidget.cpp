#include "GLDEditorWidget.h"
#include "qdatetime.h"
#include "qfileinfo.h"
#include "qfont.h"
#include <QStandardPaths>
#include "qmap.h"
#include "qmessagebox.h"
#include "qobject.h"
#include "qplaintextedit.h"
#include "qregularexpression.h"
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QFuture>
#include <QDebug>
#include <QRegularExpression>
#include <QTextCursor>
#include <QDateTime>

// ==================================
// ========= EditorWidget ===========
// ==================================

GLDEditorWidget::GLDEditorWidget(QWidget* parent)
    : QPlainTextEdit(parent)
    , currentFilePath("untitled")
    , m_statusBarTextCursor("")
    , m_statusBarTextFileInfo("")
    , m_isModify(false)
    , appDataPath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/RencentFiles")
    , m_isCaseSensitive(false)
    , m_isWordMatch(false)
{

    // Init Editor
    new Highlighter(this->document());

    int fontId = QFontDatabase::addApplicationFont(":/fonts/FiraCode-Regular.ttf");
    if (fontId != -1)
    {
        QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
        QFont   font(fontFamily, 10);
        this->setFont(font);
    }
    else
    {
        qDebug() << "Load font failed!";
    }

    lineNumber.clear();
    currentMatchIndex = -1;

    connect(this, &QPlainTextEdit::cursorPositionChanged, this, [this]() { updateCursorPosition(); });

    connect(this, &QPlainTextEdit::textChanged, this, [this]() {
        m_isModify = true;
        emit signalUpdateFileName(currentFilePath, m_isModify);
    });

    readRecentFiles();

    newFile();
}

void GLDEditorWidget::newFile()
{
    if (!handleCloseFile())
        return;

    currentFilePath = "untitled";
    this->clear();

    QString initText = "HEADER;\nFILE_DESCRIPTION(('GFC3X2'),'65001');";
    initText += "\nFILE_NAME('" + currentFilePath + "',";
    initText += "'" + QDateTime::currentDateTime().toString() + "',";
    initText += "(''), ('Glodon'), 'objectbuf', '', '');";
    initText += "\nFILE_SCHEMA(('GFC3X2'));\nENDSEC;\nADS;\nDATA;\n";
    initText += appDataPath;
    this->appendPlainText(initText);

    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::Start);
    setTextCursor(cursor);

    m_isModify = false;
    // Update filename and cursor position
    emit signalUpdateFileName(currentFilePath, m_isModify);
    updateCursorPosition();
    updateFileInfo(currentFilePath);
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
            setPlainText(in.readAll());
            currentFilePath = fileName;
            file.close();

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
            out << toPlainText();
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
            out << toPlainText();
            currentFilePath = fileName;
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

QPair<int, int> GLDEditorWidget::findAllText(QString pText, bool isCaseSensitive, bool isWordMatch)
{
    highlightCurrentMatch(QPair<int, int>(-1, -1));
    if (pText.isEmpty())
    {
        lineNumber.clear();
        return QPair<int, int>(-1, -1);
    }

    if (pText == m_currentFindText && !lineNumber.isEmpty() && isCaseSensitive == m_isCaseSensitive && isWordMatch == m_isWordMatch)
    {
        return lineNumber.at(currentMatchIndex);
    }

    m_isCaseSensitive = isCaseSensitive;
    m_isWordMatch     = isWordMatch;

    lineNumber.clear();
    currentMatchIndex = -1;
    m_currentFindText = pText;
    QString text      = toPlainText();

    // 构建正则表达式
    QRegularExpression re(QRegularExpression::escape(m_currentFindText), QRegularExpression::PatternOptions(QRegularExpression::NoPatternOption));
    QTextCursor        cursor(document());

    while (!cursor.atEnd())
    {
        QTextDocument::FindFlags flags;
        if (m_isWordMatch)
            flags |= QTextDocument::FindWholeWords;
        if (m_isCaseSensitive)
            flags |= QTextDocument::FindCaseSensitively;

        cursor = document()->find(re, cursor, flags);

        if (cursor.isNull())
        {
            break;
        }
        int lineNum = cursor.blockNumber() + 1;  // 行号从1开始计数

        lineNumber.append(QPair<int, int>(lineNum, cursor.positionInBlock() - m_currentFindText.length() + 1));
    }


    QTextCursor curCursor = textCursor();
    int         line      = curCursor.blockNumber();  // 行号从0开始计数

    QPair<int, int> closestMatch(-1, -1);
    int             minDiff = document()->lineCount();
    for (const auto& pair : lineNumber)
    {
        int diff = pair.first - line;
        if (diff >= 0 && diff < minDiff)
        {
            minDiff      = diff;
            closestMatch = pair;
        }
    }

    if (closestMatch != QPair<int, int>(-1, -1))
    {
        currentMatchIndex = lineNumber.indexOf(closestMatch);
        highlightCurrentMatch(closestMatch);
    }
    return closestMatch;
}


QPair<int, int> GLDEditorWidget::findNextText()
{
    if (lineNumber.isEmpty())
    {
        return QPair<int, int>(-1, -1);
    }

    ++currentMatchIndex;
    if (currentMatchIndex >= lineNumber.size())
    {
        currentMatchIndex = 0;
    }

    QPair<int, int> matchIndex = lineNumber.at(currentMatchIndex);

    // 高亮显示当前匹配项
    highlightCurrentMatch(matchIndex);
    return matchIndex;
}

QPair<int, int> GLDEditorWidget::findPreviousText()
{
    if (lineNumber.isEmpty())
    {
        return QPair<int, int>(-1, -1);
    }

    --currentMatchIndex;
    if (currentMatchIndex < 0)
    {
        currentMatchIndex = lineNumber.size() - 1;
    }

    QPair<int, int> matchIndex = lineNumber.at(currentMatchIndex);

    // 高亮显示当前匹配项
    highlightCurrentMatch(matchIndex);
    return matchIndex;
}

QMap<QPair<int, int>, QString> GLDEditorWidget::getMatchedList()
{
    QMap<QPair<int, int>, QString> res;
    QTextDocument*                 document = this->document();
    // 使用行号获取文本块
    for (const auto& it : this->lineNumber)
    {
        QTextBlock block = document->findBlockByLineNumber(it.first - 1);
        res.insert(it, block.text());
    }
    return res;
}

int GLDEditorWidget::getCurrentMatchNumber()
{
    return currentMatchIndex;
}

void GLDEditorWidget::replaceText(QString pText, QString rText)
{
    // if (lineNumber.isEmpty() || currentLine < 0 || currentLine >= lineNumber.size())
    // {
    //     qDebug() << "没有可替换的内容！";
    //     return;
    // }

    // QTextCursor cursor = textCursor();
    // cursor.beginEditBlock();

    // int lineNum = lineNumber.at(currentLine);
    // cursor.movePosition(QTextCursor::Start);
    // cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, lineNum - 1);
    // cursor.select(QTextCursor::LineUnderCursor);
    // QString line = cursor.selectedText();

    // // 使用正则表达式替换，以保持与查找逻辑一致
    // QRegularExpression re(QRegularExpression::escape(pText));
    // QString            newLine = line.replace(re, rText);

    // cursor.removeSelectedText();
    // cursor.insertText(newLine);

    // cursor.endEditBlock();

    // // 更新搜索结果
    // findAllText(m_currentFindText, true, false);  // 假设使用区分大小写和非全词匹配

    // qDebug() << "已在第" << lineNum << "行完成替换";
}

// 新增辅助函数
void GLDEditorWidget::highlightCurrentMatch(QPair<int, int> matchIndex)
{
    if (matchIndex.first == -1)
    {
        // 取消高亮
        QTextCursor cursor = textCursor();
        cursor.clearSelection();
        setTextCursor(cursor);
    }
    else
    {
        QTextCursor cursor = textCursor();
        cursor.movePosition(QTextCursor::Start);
        cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, matchIndex.first - 1);
        cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, matchIndex.second - 1);
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, m_currentFindText.length());

        setTextCursor(cursor);
        ensureCursorVisible();
    }
}

void GLDEditorWidget::updateCursorPosition()
{
    QTextCursor cursor = textCursor();

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
        setPlainText(file.readAll());
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


// Read fileinfo from cache
void GLDEditorWidget::readRecentFiles()
{
    m_recentFiles.clear();

    QFile file(appDataPath);

    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream in(&file);

        int     lineCount = 0;
        QString line;
        while (!in.atEnd() && lineCount < 5)
        {
            line = in.readLine();
            m_recentFiles.enqueue(line);
            lineCount++;
        }
        file.close();
    }
}

// Clear file and write in
void GLDEditorWidget::writeRecentFiles()
{
    QFile file(appDataPath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        QTextStream out(&file);
        while (!m_recentFiles.isEmpty())
        {
            out << m_recentFiles.dequeue() << "\n";
        }
        file.close();
    }
}

bool GLDEditorWidget::setEditorFontFamily(const QString& fontFamily)
{
    QFont font(fontFamily);
    setFont(font);
    return true;
}

bool GLDEditorWidget::setFontSize(int size)
{
    QFont font(this->font());
    font.setPointSize(size);
    setFont(font);
    return true;
}

GLDEditorWidget::~GLDEditorWidget()
{
    writeRecentFiles();
}