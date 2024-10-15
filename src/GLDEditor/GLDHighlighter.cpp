#include "GLDHighlighter.h"

#include <QRegularExpression>

// ==================================
// ========= Highlighter ============
// ==================================
Highlighter::Highlighter(QTextDocument* parent)
    : QSyntaxHighlighter(parent)
{
    // 设置关键字高亮
    HighlightingRule rule;

    // 匹配纯数字
    QTextCharFormat numberFormat;
    numberFormat.setForeground(QColor("#69584f"));
    rule.pattern = QRegularExpression(R"(\b\d+\b)");  // 匹配纯数字
    rule.format  = numberFormat;
    highlightingRules.append(rule);

    // 定义对象（如 #6）的格式
    QTextCharFormat objectFormat;
    objectFormat.setForeground(QColor("#c28257"));
    objectFormat.setFontWeight(QFont::Bold);

    rule.pattern = QRegularExpression(R"(#\d+)");  // 匹配类似 #6 的对象
    rule.format  = objectFormat;
    highlightingRules.append(rule);

    // 定义类名（如 GFCFLOOR）的格式
    QTextCharFormat classFormat;
    classFormat.setForeground(QColor("#6691cd"));
    classFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression(R"(\bGFC[A-Z0-9]+\b)");  // 匹配类名（以 GFC 开头的大写单词或数字）
    rule.format  = classFormat;
    highlightingRules.append(rule);

    // 单引号
    QTextCharFormat singleQuoteContentFormat;
    singleQuoteContentFormat.setForeground(QColor("#718773"));
    QRegularExpression pattern(R"('([^']|'')*')");
    highlightingRules.append({ pattern, singleQuoteContentFormat });

    // 定义完整单词$的格式
    QTextCharFormat dollarFormat;
    dollarFormat.setForeground(QColor("#ce94b9"));
    rule.pattern = QRegularExpression(R"(\b\$\b)");  // 匹配完整单词$
    rule.format  = dollarFormat;
    highlightingRules.append(rule);
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
