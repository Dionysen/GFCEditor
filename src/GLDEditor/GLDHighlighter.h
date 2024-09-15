
#include <QRegularExpression>

#include <QSyntaxHighlighter>
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
