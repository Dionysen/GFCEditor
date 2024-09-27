#ifndef GLDSEARCHWINDOW_H
#define GLDSEARCHWINDOW_H
#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QTextEdit>
#include <QIcon>
#include <QStringListModel>
#include <QCheckBox>
#include <QVector>

class GLDSearchWindow : public QWidget
{
    Q_OBJECT

  public:
    GLDSearchWindow(QWidget* parent = nullptr);

    // 高亮文本
    QString highlightText(const QString& line, const QString& searchText);

    // 更新搜索结果的槽函数（供外部调用）
    void setSearchResults(const QMap<int, QString>& results);

    // 搜索框文本变化信号
  signals:
    // 参数列表（搜索文本， 是否向前搜索， 是否选择区分大小写， 是否选择全字匹配）
    void signalPrevSearch(const QString& searchText, const bool caseSensitive, const bool wholeWord);
    void signalNextSearch(const QString& searchText, const bool caseSensitive, const bool wholeWord);
    void signalReplace(const QString& searchText, const QString& replaceText, const bool caseSensitive, const bool wholeWord);

    // 双击条目在编辑框中定位
    void signalClickRow(const int row);

  private:
    QString m_searchText;  // 要搜索的文本


    QLabel* m_pSearchIcon;
    QLabel* m_pSearchLabel;
    QLabel* m_pReplaceIcon;
    QLabel* m_pReplaceLabel;

    QLineEdit* m_pSearchEdit;
    QLineEdit* m_pReplaceEdit;

    QPushButton* m_pPrevButton;
    QPushButton* m_pNextButton;
    QPushButton* m_pReplaceButton;

    QListWidget* m_pResultList;  // 查找结果列表

    QCheckBox* m_pCaseSensitiveCheckBox;
    QCheckBox* m_pWholeWordCheckBox;

    QVBoxLayout* mainLayout;

    QVector<int> m_RowsList;  // 查找结果行号
};

#endif  // GLDSEARCHWINDOW_H
