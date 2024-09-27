#include "GLDEditorWidget.h"
#include "qdatetime.h"
#include "qdockwidget.h"
#include "qfileinfo.h"
#include "qfont.h"
#include <QStandardPaths>
#include "qmap.h"
#include "qmessagebox.h"
#include "qobject.h"
#include "qplaintextedit.h"
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

GLDEditorWidget::GLDEditorWidget(const QString& title, QWidget* parent)
    : QDockWidget(title, parent)
    , currentFilePath("untitled")
    , m_statusBarTextCursor("")
    , m_statusBarTextFileInfo("")
    , m_isModify(false)
    , appDataPath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/RencentFiles")
    , m_isCaseSensitive(false)
    , m_isWordMatch(false)
{

    // Init Editor
    p_textEditor = new QPlainTextEdit(this);
    new Highlighter(this->p_textEditor->document());

    int fontId = QFontDatabase::addApplicationFont(":/fonts/FiraCode-Regular.ttf");
    if (fontId != -1)
    {
        QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
        QFont   font(fontFamily, 10);
        p_textEditor->setFont(font);
    }
    else
    {
        qDebug() << "Load font failed!";
    }

    // Layout
    setAllowedAreas(Qt::AllDockWidgetAreas);
    setWidget(p_textEditor);
    setTitleBarWidget(new QWidget());

    lineNumber.clear();
    currentLine = -1;

    connect(this->p_textEditor, &QPlainTextEdit::cursorPositionChanged, this, [this]() { updateCursorPosition(); });

    connect(this->p_textEditor, &QPlainTextEdit::textChanged, this, [this]() {
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
    this->p_textEditor->clear();

    QString initText = "HEADER;\nFILE_DESCRIPTION(('GFC3X2'),'65001');";
    initText += "\nFILE_NAME('" + currentFilePath + "',";
    initText += "'" + QDateTime::currentDateTime().toString() + "',";
    initText += "(''), ('Glodon'), 'objectbuf', '', '');";
    initText += "\nFILE_SCHEMA(('GFC3X2'));\nENDSEC;\nADS;\nDATA;\n";
    initText += appDataPath;
    this->p_textEditor->appendPlainText(initText);

    QTextCursor cursor = p_textEditor->textCursor();
    cursor.movePosition(QTextCursor::End);
    p_textEditor->setTextCursor(cursor);

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
            p_textEditor->setPlainText(in.readAll());
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
            out << p_textEditor->toPlainText();
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

void GLDEditorWidget::findAllText(QString pText, bool isCaseSensitive, bool isWordMatch)
{
    highlightCurrentMatch(-1);
    if (pText.isEmpty())
    {
        return;
    }

    if (pText == m_currentFindText && !lineNumber.isEmpty() && isCaseSensitive == m_isCaseSensitive && isWordMatch == m_isWordMatch)
    {
        return;
    }

    m_isCaseSensitive = isCaseSensitive;
    m_isWordMatch     = isWordMatch;

    lineNumber.clear();
    currentLine       = -1;
    m_currentFindText = pText;
    QString text      = p_textEditor->toPlainText();

    // 根据是否区分大小写和全字匹配构建正则表达式
    QRegularExpression::PatternOptions options = QRegularExpression::NoPatternOption;
    if (!isCaseSensitive)
    {
        options |= QRegularExpression::CaseInsensitiveOption;
    }
    QString patternStr = isWordMatch ? "\\b" + QRegularExpression::escape(m_currentFindText) + "\\b" : QRegularExpression::escape(m_currentFindText);
    QRegularExpression re(patternStr, options);

    QTextCursor cursor(p_textEditor->document());
    while (!cursor.atEnd())
    {
        cursor = p_textEditor->document()->find(re, cursor);
        if (cursor.isNull())
        {
            break;
        }
        int lineNum = cursor.blockNumber() + 1;  // 行号从1开始计数
        lineNumber.append(lineNum);
    }
}


int GLDEditorWidget::findNextText()
{
    if (lineNumber.isEmpty())
    {
        return -1;
    }

    ++currentLine;
    if (currentLine >= lineNumber.size())
    {
        currentLine = 0;
    }

    int lineNum = lineNumber.at(currentLine);

    // 高亮显示当前匹配项
    highlightCurrentMatch(lineNum - 1);
    return lineNum - 1;
}

int GLDEditorWidget::findPreviousText()
{
    if (lineNumber.isEmpty())
    {
        return -1;
    }

    --currentLine;
    if (currentLine < 0)
    {
        currentLine = lineNumber.size() - 1;
    }

    int lineNum = lineNumber.at(currentLine);

    // 高亮显示当前匹配项
    highlightCurrentMatch(lineNum - 1);
    return lineNum - 1;
}

QMap<int, QString> GLDEditorWidget::getMatchedList()
{
    QMap<int, QString> res;
    QTextDocument*     document = this->p_textEditor->document();
    // 使用行号获取文本块
    for (const auto& it : this->lineNumber)
    {
        QTextBlock block = document->findBlockByLineNumber(it);
        res.insert(it, block.text());
    }
    return res;
}

int GLDEditorWidget::getCurrentMatchNumber()
{
    return currentLine;
}

void GLDEditorWidget::replaceText(QString pText, QString rText)
{
    if (lineNumber.isEmpty() || currentLine < 0 || currentLine >= lineNumber.size())
    {
        qDebug() << "没有可替换的内容！";
        return;
    }

    QTextCursor cursor = p_textEditor->textCursor();
    cursor.beginEditBlock();

    int lineNum = lineNumber.at(currentLine);
    cursor.movePosition(QTextCursor::Start);
    cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, lineNum - 1);
    cursor.select(QTextCursor::LineUnderCursor);
    QString line = cursor.selectedText();

    // 使用正则表达式替换，以保持与查找逻辑一致
    QRegularExpression re(QRegularExpression::escape(pText));
    QString            newLine = line.replace(re, rText);

    cursor.removeSelectedText();
    cursor.insertText(newLine);

    cursor.endEditBlock();

    // 更新搜索结果
    findAllText(m_currentFindText, true, false);  // 假设使用区分大小写和非全词匹配

    qDebug() << "已在第" << lineNum << "行完成替换";
}

// 新增辅助函数
void GLDEditorWidget::highlightCurrentMatch(int lineNum)
{
    if (lineNum == -1)
    {
        // 取消高亮
        QTextCursor cursor = p_textEditor->textCursor();
        cursor.clearSelection();
        p_textEditor->setTextCursor(cursor);
    }
    else
    {
        QTextCursor cursor = p_textEditor->textCursor();
        cursor.movePosition(QTextCursor::Start);
        cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, lineNum);
        cursor.select(QTextCursor::LineUnderCursor);

        p_textEditor->setTextCursor(cursor);
        p_textEditor->ensureCursorVisible();
    }
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

bool GLDEditorWidget::setFontFamily(const QString& fontFamily)
{
    QFont font(fontFamily);
    p_textEditor->setFont(font);
    return true;
}

bool GLDEditorWidget::setFontSize(int size)
{
    QFont font(p_textEditor->font());
    font.setPointSize(size);
    p_textEditor->setFont(font);
    return true;
}

GLDEditorWidget::~GLDEditorWidget()
{
    writeRecentFiles();
}