#include "MainWindow.h"
#include "GLDAttributeArea.h"
#include "GLDAuxiliaryArea.h"
#include "GLDSearchWindow.h"
#include "GLDToolBar.h"
#include "ads/API.h"
#include "ads/ContainerWidget.h"
#include "ads/SectionContent.h"
#include "ads/SectionContentWidget.h"
#include "qaction.h"
#include "qapplication.h"
#include "qchar.h"
#include "qdockwidget.h"
#include "qfileinfo.h"
#include "qlabel.h"
#include "qmap.h"
#include "qnamespace.h"
#include "qobject.h"
#include "qsizepolicy.h"
#include "qtextedit.h"
#include "qtoolbar.h"
#include "qwidget.h"
#include "ui_MainWindow.h"
#include <QSplitter>
#include <QMessageBox>
#include <QScrollArea>
#include <QLabel>
#include <QTextStream>
#include <QFont>
#include <QDockWidget>
#include <QStyle>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    GetMainWindow()->resize(1280, 800);

    // 设置样式
    QFile file(":/qss/dark.qss");
    if (file.open(QFile::ReadOnly))
    {
        QString style = file.readAll();
        qApp->setStyleSheet(style);
    }

    // 布局
    setupLayout();

    // 设置菜单栏
    setupMenuBar();

    // 设置工具栏
    setupToolBar();

    // 设置编辑器
    setupEditor();

    // 设置辅助区域
    setupAuxiliary();

    // mockdata
    QMap<QString, QVariant> propertyData;
    propertyData["Name"]   = "Example";
    propertyData["Age"]    = 30;
    propertyData["Active"] = true;
    p_attributeArea->setProperties(propertyData);
}
void MainWindow::setupEditor()
{
    connect(p_editorWidget, &GLDEditorWidget::signalUpdateStatusBar, this,
            [this](QString cursor, QString fileInfo) { ui->statusBar->showMessage(cursor + QStringLiteral("      ") + fileInfo); });

    connect(p_editorWidget, &GLDEditorWidget::signalUpdateFileName, this, [this](const QString& filename, bool isModify) {
        if (isModify)
            setWindowTitle(filename + "*");
        else
            setWindowTitle(filename);
    });
}

void MainWindow::setupLayout()
{
    // ========= 布局管理 ========
    setDockNestingEnabled(true);

    // ========= schema =========
    p_schemaWiddget = new GLDSchemaWidget(this);

    schemaDockWidget = new QDockWidget(QStringLiteral("视图区"), this);
    schemaDockWidget->setAllowedAreas(Qt::AllDockWidgetAreas);
    schemaDockWidget->setWidget(p_schemaWiddget);
    schemaDockWidget->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);

    // ========= 辅助区 =========
    p_auxiliaryArea = new GLDAuxiliaryArea(this);

    auxiliarydockWidget = new QDockWidget(QStringLiteral("辅助区"), this);
    auxiliarydockWidget->setAllowedAreas(Qt::AllDockWidgetAreas);
    auxiliarydockWidget->setWidget(p_auxiliaryArea);
    auxiliarydockWidget->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);


    // ========= 属性区 =========
    p_attributeArea = new GLDAttributeArea(this);

    attributedockWidget = new QDockWidget(QStringLiteral("属性区"), this);
    attributedockWidget->setAllowedAreas(Qt::AllDockWidgetAreas);
    attributedockWidget->setWidget(p_attributeArea);
    attributedockWidget->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);

    // ========= editor =========
    p_editorWidget = new GLDEditorWidget(this);

    editorDockWidget = new QDockWidget(this);
    editorDockWidget->setWidget(p_editorWidget);
    editorDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    editorDockWidget->setTitleBarWidget(new QWidget());

    // 设置布局
    addDockWidget(Qt::LeftDockWidgetArea, schemaDockWidget);
    splitDockWidget(schemaDockWidget, editorDockWidget, Qt::Horizontal);
    splitDockWidget(editorDockWidget, attributedockWidget, Qt::Horizontal);
    splitDockWidget(editorDockWidget, auxiliarydockWidget, Qt::Vertical);

    // 设置dock的初始大小
    resizeDocks({ schemaDockWidget }, { 150 }, Qt::Horizontal);
    resizeDocks({ attributedockWidget }, { 150 }, Qt::Horizontal);
    resizeDocks({ auxiliarydockWidget }, { 150 }, Qt::Vertical);

    // 删除centralwidget，所有部件均为dockwidget
    QWidget* p = takeCentralWidget();
    if (p)
        delete p;
}

// 设置菜单栏样式
void MainWindow::setupMenuBar()
{
    refresh = new QAction(this);
    refresh->setText("Refresh");

    connect(refresh, &QAction::triggered, this, [this]() {
        QFile file(":/qss/dark.qss");

        if (file.open(QFile::ReadOnly))
        {
            QString style = file.readAll();
            qApp->setStyleSheet(style);
        }
    });

    ui->actionNew->setIcon(QIcon(QString(":/image/new.png")));
    ui->actionOpen->setIcon(QIcon(QString(":/image/open.png")));
    ui->actionSave->setIcon(QIcon(QString(":/image/save.png")));

    ui->actionSaveAs->setIcon(QIcon(QString(":/image/saveas.png")));
    ui->actionExit->setIcon(QIcon(QString(":/image/exit.png")));

    ui->actionSaveAs->setIcon(QIcon(QString(":/image/new.png")));

    ui->actionExit->setIcon(QIcon(QString(":/image/new.png")));
    ui->actionRedo->setIcon(QIcon(QString(":/image/redo.png")));
    ui->actionUndo->setIcon(QIcon(QString(":/image/undo.png")));
    ui->actionCut->setIcon(QIcon(QString(":/image/cut.png")));
    ui->actionCopy->setIcon(QIcon(QString(":/image/copy.png")));
    ui->actionPaste->setIcon(QIcon(QString(":/image/paste.png")));
    ui->actionFind->setIcon(QIcon(QString(":/image/find.png")));
    ui->actionFindNext->setIcon(QIcon(QString(":/image/findnext.png")));
    ui->actionReplace->setIcon(QIcon(QString(":/image/replace.png")));

    ui->actionBackword->setIcon(QIcon(QString(":/image/backward.png")));
    ui->actionForward->setIcon(QIcon(QString(":/image/forward.png")));
    ui->actionLocation->setIcon(QIcon(QString(":/image/location.png")));

    ui->menuView->setIcon(QIcon(QString(":/image/visible.png")));

    ui->actionColor->setIcon(QIcon(QString(":/image/color.png")));
    ui->actionCite->setIcon(QIcon(QString(":/image/cite.png")));
    ui->actionCheck->setIcon(QIcon(QString(":/image/check.png")));

    ui->actionHelp->setIcon(QIcon(QString(":/image/help.png")));
    ui->actionAbout->setIcon(QIcon(QString(":/image/about.png")));

    // TODO: Icons Download

    connect(ui->actionOpen, &QAction::triggered, this, [this]() { p_editorWidget->openFile(); });
    connect(ui->actionSave, &QAction::triggered, this, [this]() { p_editorWidget->saveFile(); });
    connect(ui->actionSaveAs, &QAction::triggered, this, [this]() { p_editorWidget->saveAs(); });

    connect(ui->actionCut, &QAction::triggered, this, [this]() { p_editorWidget->getTextEditor()->cut(); });
    connect(ui->actionPaste, &QAction::triggered, this, [this]() { p_editorWidget->getTextEditor()->paste(); });
    connect(ui->actionCopy, &QAction::triggered, this, [this]() { p_editorWidget->getTextEditor()->copy(); });

    connect(ui->actionRedo, &QAction::triggered, this, [this]() { p_editorWidget->getTextEditor()->redo(); });
    connect(ui->actionUndo, &QAction::triggered, this, [this]() { p_editorWidget->getTextEditor()->undo(); });

    connect(ui->actionExit, &QAction::triggered, this, [this]() { QApplication::exit(0); });

    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::on_actionAbout_clicked);
    connect(ui->actionHelp, &QAction::triggered, this, &MainWindow::on_actionHelp_clicked);

    ui->actionNew->setShortcut(QKeySequence("Ctrl+N"));
    ui->actionOpen->setShortcut(QKeySequence("Ctrl+O"));
    ui->actionSave->setShortcut(QKeySequence("Ctrl+S"));
    ui->actionSaveAs->setShortcut(QKeySequence("Ctrl+Shift+S"));
    ui->actionExit->setShortcut(QKeySequence("Ctrl+Q"));
    ui->actionCut->setShortcut(QKeySequence("Ctrl+X"));
    ui->actionCopy->setShortcut(QKeySequence("Ctrl+C"));
    ui->actionPaste->setShortcut(QKeySequence("Ctrl+V"));
    ui->actionFind->setShortcut(QKeySequence("Ctrl+F"));
    ui->actionFindNext->setShortcut(QKeySequence("Ctrl+Shift+F"));
    ui->actionReplace->setShortcut(QKeySequence("Ctrl+R"));

    // 可见性
    connect(ui->actionAtrtri, &QAction::triggered, this, [this]() {
        if (!attributedockWidget->isHidden())
            attributedockWidget->hide();
        else
            attributedockWidget->show();
    });
    connect(ui->actionView, &QAction::triggered, this, [this]() {
        if (!schemaDockWidget->isHidden())
            schemaDockWidget->hide();
        else
            schemaDockWidget->show();
    });
    connect(ui->actionAsiss, &QAction::triggered, this, [this]() {
        if (!auxiliarydockWidget->isHidden())
            auxiliarydockWidget->hide();
        else
            auxiliarydockWidget->show();
    });
    connect(ui->actionStatus, &QAction::triggered, this, [this]() {
        if (!ui->statusBar->isHidden())
            ui->statusBar->hide();
        else
            ui->statusBar->show();
    });
    connect(ui->actionToolbar, &QAction::triggered, this, [this]() {
        if (!p_toolBar->isHidden())
            p_toolBar->hide();
        else
            p_toolBar->show();
    });
}

void MainWindow::setupToolBar()
{
    p_toolBar = new GLDToolBar();
    this->addToolBar(p_toolBar);
    connect(this->p_toolBar->p_new, &QAction::triggered, this, [this]() { p_editorWidget->newFile(); });
    connect(this->p_toolBar->p_open, &QAction::triggered, this, [this]() { p_editorWidget->openFile(); });

    connect(this->p_toolBar->p_save, &QAction::triggered, this, [this]() { p_editorWidget->saveFile(); });
    connect(this->p_toolBar->p_cut, &QAction::triggered, this, [this]() { p_editorWidget->getTextEditor()->cut(); });
    connect(this->p_toolBar->p_copy, &QAction::triggered, this, [this]() { p_editorWidget->getTextEditor()->copy(); });
    connect(this->p_toolBar->p_paste, &QAction::triggered, this, [this]() { p_editorWidget->getTextEditor()->paste(); });
    connect(this->p_toolBar->p_redo, &QAction::triggered, this, [this]() { p_editorWidget->getTextEditor()->redo(); });
    connect(this->p_toolBar->p_undo, &QAction::triggered, this, [this]() { p_editorWidget->getTextEditor()->undo(); });

    // TODO
    connect(this->p_toolBar->p_find, &QAction::triggered, this, [this]() {});
    connect(this->p_toolBar->p_replace, &QAction::triggered, this, [this]() {});

    connect(p_editorWidget, &GLDEditorWidget::signalUpdateRecentFiles, this, [this](QQueue<QString>& recentFiles) {
        ui->menuRecent->clear();
        for (const auto& it : recentFiles)
        {
            QAction* action = new QAction(it, this);
            ui->menuRecent->addAction(action);
        }
    });

    connect(this->p_toolBar->p_refresh, &QAction::triggered, this, [this]() {
        QFile file("vendor/QtCustomTitlebarWindow/assets/qss/dark.qss");
        if (file.open(QFile::ReadOnly))
        {
            QString style = file.readAll();
            qApp->setStyleSheet(style);
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_clicked()
{
    // 创建一个QMessageBox对象，显示About相关信息
    QMessageBox messageAbout;
    messageAbout.setWindowTitle(QStringLiteral("关于"));
    messageAbout.setText(QStringLiteral("<h2>C++大作业</h2>"
                                        "<p>版权所有 © 20240830 [TOT24 C++班 第五组]。</p>"));

    messageAbout.setIcon(QMessageBox::NoIcon);                        // 先移除默认图标
    messageAbout.setWindowIcon(QIcon(QString(":/image/about.png")));  // 设置自定义图标
    messageAbout.exec();
}

void MainWindow::on_actionHelp_clicked()
{
    QDialog dialogHelp;
    dialogHelp.setWindowIcon(QIcon(QString(":/image/help.png")));  // 设置自定义图标

    // 设置对话框标题
    dialogHelp.setWindowTitle(QStringLiteral("帮助文档"));

    // 滚动区域
    QScrollArea* scrollArea = new QScrollArea(&dialogHelp);
    scrollArea->setWidgetResizable(true);

    // 显示帮助文档
    // QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QWidget*   contentWidget = new QWidget(scrollArea);
    QTextEdit* textEdit      = new QTextEdit(contentWidget);

    QString fileName = QStringLiteral("../帮助文档.txt");
    QFile   file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        textEdit->setPlainText((QStringLiteral("这是帮助信息\n")));
    }
    else
    {
        QTextStream in(&file);
        textEdit->setPlainText(in.readAll());
    }
    scrollArea->setWidget(contentWidget);

    // 将QTextEdit添加到布局中
    QVBoxLayout* layout = new QVBoxLayout(contentWidget);
    layout->addWidget(textEdit);

    QLabel* labelHelp = new QLabel(QStringLiteral("<p>版权所有 © 20240830 [TOT24 C++班 第五组]。</p>"));
    layout->addWidget(labelHelp);

    // 显示对话框
    dialogHelp.resize(400, 320);
    dialogHelp.exec();
}

void MainWindow::setupAuxiliary()
{
    connect(p_auxiliaryArea->m_searchWindow, &GLDSearchWindow::signalNextSearch, this,
            [this](const QString& searchText, const bool prevs, const bool caseSensitive, const bool wholeWord) {
                p_editorWidget->findAllText(searchText, caseSensitive, wholeWord);
                if (prevs)
                {
                    p_editorWidget->findPreviousText();
                }
                else
                {
                    p_editorWidget->findNextText();
                }

                QMap<int, QString> res;
                QTextDocument*     document = p_editorWidget->getTextEditor()->document();

                // 使用行号获取文本块

                for (const auto& it : p_editorWidget->lineNumber)
                {

                    QTextBlock block = document->findBlockByLineNumber(it);
                    res[it]          = block.text();
                }

                p_auxiliaryArea->setSearchResults(res);
            });

    connect(ui->actionFind, &QAction::triggered, this, [this]() { auxiliarydockWidget->show(); });
    connect(ui->actionFindNext, &QAction::triggered, this, [this]() { auxiliarydockWidget->show(); });
    connect(ui->actionReplace, &QAction::triggered, this, [this]() { auxiliarydockWidget->show(); });
}
