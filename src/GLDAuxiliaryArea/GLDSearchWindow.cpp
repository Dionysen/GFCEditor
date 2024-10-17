#include "GLDSearchWindow.h"
#include <QCheckBox>
#include "GLDStyleManager.h"
#include <QPixmap>

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
    m_pSearchIcon = new QLabel(this);
    m_pSearchIcon->setFixedSize(m_LineHeight - 5, m_LineHeight);
    // 替换图标
    m_pReplaceIcon = new QLabel(this);
    m_pReplaceIcon->setFixedSize(m_LineHeight - 5, m_LineHeight);
    // 搜索框
    m_pSearchEdit = new QLineEdit(this);
    m_pSearchEdit->setPlaceholderText("Enter text to search...");
    m_pSearchEdit->setMinimumHeight(m_LineHeight);

    // 区分大小写
    m_pCaseSensitiveCheckBox = new QCheckBox(QStringLiteral("Match Case"), this);
    // 全字匹配
    m_pWholeWordCheckBox = new QCheckBox(QStringLiteral("Match Whole Word"), this);

    // 查找上一个按钮
    m_pPrevButton = new QPushButton("Previous", this);
    m_pPrevButton->setMinimumHeight(m_LineHeight);
    m_pPrevButton->setMinimumWidth(m_BtnWidth);
    // 查找下一个按钮
    m_pNextButton = new QPushButton("Next", this);
    m_pNextButton->setMinimumHeight(m_LineHeight);
    m_pNextButton->setMinimumWidth(m_BtnWidth);

    // 替换框
    m_pReplaceEdit = new QLineEdit(this);
    m_pReplaceEdit->setPlaceholderText("Enter text to replace...");
    m_pReplaceEdit->setMinimumHeight(m_LineHeight);


    // 替换按钮
    m_pReplaceButton = new QPushButton("Replace", this);
    m_pReplaceButton->setMinimumHeight(m_LineHeight);
    m_pReplaceButton->setMinimumWidth(m_BtnWidth);
    // 将控件添加到布局中
    searchLayout->addSpacing(10);
    searchLayout->addWidget(m_pSearchIcon);
    searchLayout->addWidget(m_pSearchEdit);
    searchLayout->addWidget(m_pCaseSensitiveCheckBox);
    searchLayout->addWidget(m_pWholeWordCheckBox);
    searchLayout->addWidget(m_pPrevButton);
    searchLayout->addWidget(m_pNextButton);

    searchLayout->setContentsMargins(0, 0, 0, 0);

    replaceLayout->addSpacing(10);
    replaceLayout->addWidget(m_pReplaceIcon);
    replaceLayout->addWidget(m_pReplaceEdit);
    replaceLayout->addWidget(m_pReplaceButton);

    // 第三行布局：查找结果列表
    m_pResultList = new QListWidget(this);
    m_pResultList->setFocusPolicy(Qt::NoFocus);                       // 取消鼠标点击时的虚线框
    m_pResultList->setSelectionMode(QAbstractItemView::NoSelection);  // 取消鼠标点击时的选中效果

    // 将布局添加到主布局中
    mainLayout->addSpacing(7);
    mainLayout->addLayout(searchLayout);
    mainLayout->addLayout(replaceLayout);
    mainLayout->addWidget(m_pResultList);

    mainLayout->setContentsMargins(0, 0, 0, 0);
    // 设置主布局
    setLayout(mainLayout);


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
        // 进行区分大小写的搜索
        bool caseSensitive = m_pCaseSensitiveCheckBox->isChecked();
        // 如果选择了“全字匹配”
        bool wholeWord = m_pWholeWordCheckBox->isChecked();

        // 发送信号到文本编辑区
        emit signalPrevSearch(m_pSearchEdit->text(), caseSensitive, wholeWord);
    });

    connect(m_pNextButton, &QPushButton::clicked, this, [=]() {
        // 进行区分大小写的搜索
        bool caseSensitive = m_pCaseSensitiveCheckBox->isChecked();
        // 如果选择了“全字匹配”
        bool wholeWord = m_pWholeWordCheckBox->isChecked();

        // 发送信号到文本编辑区
        emit signalNextSearch(m_pSearchEdit->text(), caseSensitive, wholeWord);
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


    connect(m_pSearchEdit, &QLineEdit::textChanged, this, [=](const QString& text) {
        emit signalSearchTextChanged(text, m_pCaseSensitiveCheckBox->isChecked(), m_pWholeWordCheckBox->isChecked());
    });

    connect(m_pReplaceEdit, &QLineEdit::textChanged, this, [=](const QString& text) {
        emit signalReplaceTextChanged(text, m_pCaseSensitiveCheckBox->isChecked(), m_pWholeWordCheckBox->isChecked());
    });

    connect(m_pCaseSensitiveCheckBox, &QCheckBox::checkStateChanged, this,
            [=]() { emit signalSearchTextChanged(m_pSearchEdit->text(), m_pCaseSensitiveCheckBox->isChecked(), m_pWholeWordCheckBox->isChecked()); });


    connect(m_pWholeWordCheckBox, &QCheckBox::checkStateChanged, this,
            [=]() { emit signalSearchTextChanged(m_pSearchEdit->text(), m_pCaseSensitiveCheckBox->isChecked(), m_pWholeWordCheckBox->isChecked()); });


    connect(GLDStyleManager::StyleManager(), &GLDStyleManager::themeChanged, this, &GLDSearchWindow::setIcon);
}


void GLDSearchWindow::setIcon()
{
    QPixmap searchIconPixmap(GLDStyleManager::StyleManager()->getIcon("find").pixmap(m_LineHeight / 2, m_LineHeight / 2));
    m_pSearchIcon->setPixmap(searchIconPixmap);
    QPixmap replaceIconPixmap(GLDStyleManager::StyleManager()->getIcon("replace").pixmap(m_LineHeight / 2, m_LineHeight / 2));
    m_pReplaceIcon->setPixmap(replaceIconPixmap);
}

void GLDSearchWindow::setSearchResults(const QMap<QPair<int, int>, QString>& results)
{
    delete m_pResultList;
    m_pResultList = new QListWidget(this);
    mainLayout->addWidget(m_pResultList);

    m_RowsList.clear();

    for (auto it = results.begin(); it != results.end(); ++it)
    {
        m_RowsList.append(it.key().first);  // 收集行号

        QListWidgetItem* item = new QListWidgetItem();
        m_pResultList->addItem(item);
        item->setText(it.value());
    }
}
