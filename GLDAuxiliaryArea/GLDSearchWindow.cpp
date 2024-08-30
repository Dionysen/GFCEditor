#include "GLDSearchWindow.h"
// #include "GLDEditorWidget.h"
// #include <qDebug>

GLDSearchWindow::GLDSearchWindow(QWidget* parent)
    : QWidget(parent)
{

    // 垂直布局（用于容纳所有控件）
    mainLayout = new QVBoxLayout;

    // 第一行布局：放大镜图标、搜索框、查找上一个按钮、查找下一个按钮
    QHBoxLayout* searchLayout = new QHBoxLayout;

    // 第二行布局：替换图标、替换框、替换按钮
    QHBoxLayout* replaceLayout = new QHBoxLayout;

    // 放大镜图标
    m_pSearchIcon = new QLabel;
    QPixmap searchIconPixmap("../bigwork/src/find.png");

    // 查找文本
    m_pSearchLabel = new QLabel(QStringLiteral("查找"));



    // 调整图片大小并保持比例
    QSize fixedSize_searchIcon(10, 10);  // 设置固定大小
    searchIconPixmap = searchIconPixmap.scaled(fixedSize_searchIcon, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_pSearchIcon->setPixmap(searchIconPixmap);

    // 搜索框
    m_pSearchEdit = new QLineEdit;
    m_pSearchEdit->setPlaceholderText("Enter text to search...");
    m_pSearchEdit->setMinimumHeight(30);

    // 区分大小写
    m_pCaseSensitiveCheckBox = new QCheckBox(QStringLiteral("区分大小写"), this);
    // 全字匹配
    m_pWholeWordCheckBox = new QCheckBox(QStringLiteral("全字匹配"), this);

    // 查找上一个按钮
    m_pPrevButton = new QPushButton("Previous");

    // 查找下一个按钮
    m_pNextButton = new QPushButton("Next");



    // 替换图标
    m_pReplaceIcon = new QLabel;
    QPixmap replaceIconPixmap("../bigwork/src/find.png");
    // 调整图片大小并保持比例
    QSize fixedSize_replaceIcon(10, 10);  // 设置固定大小
    replaceIconPixmap = replaceIconPixmap.scaled(fixedSize_replaceIcon, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_pReplaceIcon->setPixmap(replaceIconPixmap);

    // 替换文本
    m_pReplaceLabel = new QLabel(QStringLiteral("替换"));
    // 替换框
    m_pReplaceEdit = new QLineEdit;
    m_pReplaceEdit->setPlaceholderText("Enter text to replace...");

    // 替换按钮
    m_pReplaceButton = new QPushButton("Replace");

    // 将控件添加到布局中
    searchLayout->addWidget(m_pSearchIcon);
    searchLayout->addWidget(m_pSearchLabel);
    searchLayout->addWidget(m_pSearchEdit);
    searchLayout->addWidget(m_pCaseSensitiveCheckBox);
    searchLayout->addWidget(m_pWholeWordCheckBox);
    searchLayout->addWidget(m_pPrevButton);
    searchLayout->addWidget(m_pNextButton);
    searchLayout->setMargin(0);


    replaceLayout->addWidget(m_pReplaceIcon);
    replaceLayout->addWidget(m_pReplaceLabel);
    replaceLayout->addWidget(m_pReplaceEdit);
    replaceLayout->addWidget(m_pReplaceButton);

    // 第三行布局：查找结果列表
    m_pResultList = new QListWidget(this);


    // 将布局添加到主布局中
    mainLayout->addSpacing(7);
    mainLayout->addLayout(searchLayout);
    mainLayout->addLayout(replaceLayout);
    mainLayout->addWidget(m_pResultList);

    mainLayout->setMargin(0);
    // 设置主布局
    setLayout(mainLayout);

    //    // 设置窗口标题和大小
    //    setWindowTitle("Text Searcher");
    //    resize(600, 400);

    //    // 连接信号和槽函数
    //    connect(m_pSearchEdit, &QLineEdit::textChanged, this, [=]() {
    //        m_pResultList->clear();

    //        m_searchText = m_pSearchEdit->text();

    //        if (!searchText.isEmpty() && !m_textToSearch.isEmpty()) {
    //            QStringList lines = m_textToSearch.split('\n');
    //            for (int i = 0; i < lines.size(); ++i) {
    //                if (lines[i].contains(searchText, Qt::CaseInsensitive)) {
    ////                        QString line = lines[i];
    ////                        QString highlightTextLine = highlightText(line, searchText);
    ////                        QListWidgetItem *item = new QListWidgetItem(highlightTextLine);
    //                    QListWidgetItem *item = new QListWidgetItem(lines[i]);
    //                    m_pResultList->addItem(item);
    //                }
    //            }
    //        }
    //    });

    // 连接替换按钮的click信号到发送查找文本的信号
    connect(m_pReplaceButton, &QPushButton::clicked, this, [=]() {
        bool caseSensitive = false;
        bool wholeWord     = false;
        if (m_pCaseSensitiveCheckBox->isChecked())
        {
            // 进行区分大小写的搜索
            caseSensitive = true;
        }
        // 如果选择了“全字匹配”
        if (m_pWholeWordCheckBox->isChecked())
        {
            // 进行全字匹配的搜索
            wholeWord = true;
        }
        // 发送信号到文本编辑区
        emit signalReplace(m_pSearchEdit->text(), m_pReplaceEdit->text(), caseSensitive, wholeWord);
    });



    connect(m_pPrevButton, &QPushButton::clicked, this, [=]() {
        // 在查找页面进行展示
        int currentRow = m_pResultList->currentRow();

        int itemCount = m_pResultList->count();

        if (itemCount > 0)
        {
            if (currentRow > 0)
            {
                m_pResultList->setCurrentRow(currentRow - 1);
            }
            else
            {
                // 如果到达了第一行，则跳转到最后一行
                m_pResultList->setCurrentRow(itemCount - 1);
            }
        }
    });

    connect(m_pNextButton, &QPushButton::clicked, this, [=]() {
        int currentRow = m_pResultList->currentRow();
        int itemCount  = m_pResultList->count();

        if (itemCount > 0)
        {
            if (currentRow < itemCount - 1)
            {
                m_pResultList->setCurrentRow(currentRow + 1);
            }
            else
            {
                // 如果到达了最后一行，则跳转到第一行
                m_pResultList->setCurrentRow(0);
            }
        }

        bool caseSensitive = false;
        bool wholeWord     = false;
        if (m_pCaseSensitiveCheckBox->isChecked())
        {
            // 进行区分大小写的搜索
            caseSensitive = true;
        }
        // 如果选择了“全字匹配”
        if (m_pWholeWordCheckBox->isChecked())
        {
            // 进行全字匹配的搜索
            wholeWord = true;
        }

        // 发送信号到文本编辑区
        emit signalNextSearch(m_pSearchEdit->text(), false, caseSensitive, wholeWord);
    });

    // 设置样式表：突出显示当前行，并发送行号给外部
    connect(m_pResultList, &QListWidget::currentRowChanged, this, [=](int currentRow) {
        for (int i = 0; i < m_pResultList->count(); ++i)
        {
            if (i == currentRow)
            {
                m_pResultList->item(i)->setBackground(Qt::yellow);
            }
            else
            {
                m_pResultList->item(i)->setBackground(Qt::white);
            }
        }

        emit signalClickRow(m_RowsList[currentRow]);
    });
}


void GLDSearchWindow::setSearchResults(const QMap<int, QString>& results)
{
    delete m_pResultList;
    m_pResultList = new QListWidget(this);
    mainLayout->addWidget(m_pResultList);

    //    m_pResultList->clear();  // 清除之前的结果
    m_RowsList.clear();

    for (auto it = results.begin(); it != results.end(); ++it)
    {
        m_RowsList.append(it.key());  // 收集行号
        QListWidgetItem* item = new QListWidgetItem(it.value());
        m_pResultList->addItem(item);
    }
}



// 高亮文本
QString GLDSearchWindow::highlightText(const QString& line, const QString& searchText)
{
    QString highlightedLine = line;
    int     startIndex      = highlightedLine.indexOf(searchText, 0, Qt::CaseInsensitive);
    while (startIndex != -1)
    {
        highlightedLine = highlightedLine.left(startIndex) + "<font color='red'>" + highlightedLine.mid(startIndex, searchText.length()) + "</font>" +
                          highlightedLine.mid(startIndex + searchText.length());
        startIndex = highlightedLine.indexOf(searchText, startIndex + searchText.length(), Qt::CaseInsensitive);
    }
    return highlightedLine;
}
