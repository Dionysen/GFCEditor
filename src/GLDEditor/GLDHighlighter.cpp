#include <QRegularExpression>
#include "GLDHighlighter.h"

// ==================================
// ========= Highlighter ============
// ==================================
Highlighter::Highlighter(QTextDocument* parent)
    : QSyntaxHighlighter(parent)
{

    // QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

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
