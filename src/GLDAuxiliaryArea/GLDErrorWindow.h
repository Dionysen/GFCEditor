#ifndef GLDERRORWINDOW_H
#define GLDERRORWINDOW_H
#include <QListWidget>
#include <QStringList>

class GLDErrorWindow : public QWidget
{
    Q_OBJECT
  public:
    GLDErrorWindow(QWidget* parent = nullptr);

    // 更新合法性校验结果
    void updateSearchResults(const QMap<int, QString>& results);

  signals:
    // 双击条目在编辑框中定位
    void signalClickRow(const int row);

  private:
    QListWidget* m_pResultList;

    QStringList m_textToSearch;

    QVector<int> m_RowsList;
};

#endif  // GLDERRORWINDOW_H
