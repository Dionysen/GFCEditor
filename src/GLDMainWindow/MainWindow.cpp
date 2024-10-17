// GLD header
#include "MainWindow.h"
#include "CustomWindow.h"
#include "GLDAttributeArea.h"
#include "GLDAuxiliaryArea.h"
#include "GLDSearchWindow.h"
#include "GLDToolBar.h"
#include "GLDStyleManager.h"
// Qt header
#include <QAction>
#include <QApplication>
#include <QChar>
#include <QDockWidget>
#include <QFileInfo>
#include <QMap>
#include <QObject>
#include <QSizePolicy>
#include <QSplitter>
#include <QStatusBar>
#include <QTextEdit>
#include <QToolBar>
#include <QWidget>
#include <QSplitter>
#include <QMessageBox>
#include <QScrollArea>
#include <QLabel>
#include <QTextStream>
#include <QFont>
#include <QDockWidget>
#include <QStyle>
#include <QSizeGrip>
#include <QTimer>
#include "ads/API.h"
#include "ads/ContainerWidget.h"
#include "ads/SectionWidget.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{

    GetMainWindow()->resize(1200, 800);

    // Title
    auto win = GetCustomWindow();
    if (win)
        win->setWindowTitle("GFCEditor");

    p_schemaWiddget = new GLDSchemaWidget(this);
    p_auxiliaryArea = new GLDAuxiliaryArea(this);
    p_attributeArea = new GLDAttributeArea(this);
    p_editorWidget  = new GLDEditorWidget(this);

    // 布局
    setupDockingLayout();

    // 状态栏
    setupStatusBar();

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

    // 设置主题
    GLDStyleManager::StyleManager()->setTheme("dark");
    setStyle();
    connect(GLDStyleManager::StyleManager(), &GLDStyleManager::themeChanged, this, &MainWindow::setStyle);
}

// 所有与主题相关的设置，方便切换主题
void MainWindow::setStyle()
{
    qApp->setStyleSheet(GLDStyleManager::StyleManager()->getStyleSheet());
    auto win = GetCustomWindow();
    if (win)
        win->setWindowIcon(GLDStyleManager::StyleManager()->getIcon("editor"));

    // 设置所有section的图标
    for (auto it : p_container->sections())
    {
        if (it)
        {
            it->getCloseButton()->setIcon(GLDStyleManager::StyleManager()->getIcon("close"));
            it->getTabsMenuButton()->setIcon(GLDStyleManager::StyleManager()->getIcon("branchOpen"));
        }
    }
}

void MainWindow::setupStatusBar()
{
    p_StatusBar = new QStatusBar(this);
    setStatusBar(p_StatusBar);
}

void MainWindow::setupEditor()
{
    connect(p_editorWidget, &GLDEditorWidget::signalUpdateStatusBar, this,
            [this](QString cursor, QString fileInfo) { p_StatusBar->showMessage(cursor + QStringLiteral("      ") + fileInfo); });

    connect(p_editorWidget, &GLDEditorWidget::signalUpdateFileName, this, [this](const QString& filename, bool isModify) {
        if (isModify)
            setWindowTitle(filename + "*");
        else
            setWindowTitle(filename);
    });
}

void MainWindow::setupDockingLayout()
{
    // ================= Use Advanced Docking System =================
    p_container = new ADS_NS::ContainerWidget();
    // setCentralWidget(p_container);

    // ADS_NS::SectionContent::RefPtr p_EditorContent =
    //     ADS_NS::SectionContent::newSectionContent(QString("Editor"), p_container, new QLabel("Editor"), p_editorWidget);
    // p_container->addSectionContent(p_EditorContent, NULL, ADS_NS::CenterDropArea);


    // ADS_NS::SectionContent::RefPtr p_AuxiliaryContent =
    //     ADS_NS::SectionContent::newSectionContent(QString("Auxiliary"), p_container, new QLabel("Auxiliary"), p_auxiliaryArea);

    // p_container->addSectionContent(p_AuxiliaryContent, NULL, ADS_NS::BottomDropArea);

    // ADS_NS::SectionContent::RefPtr p_AttributeContent =
    //     ADS_NS::SectionContent::newSectionContent(QString("Attribute"), p_container, new QLabel("Attribute"), p_attributeArea);
    // p_container->addSectionContent(p_AttributeContent, NULL, ADS_NS::RightDropArea);

    // ADS_NS::SectionContent::RefPtr p_SchemaContent =
    //     ADS_NS::SectionContent::newSectionContent(QString("Schema"), p_container, new QLabel("Schema"), p_schemaWiddget);
    // p_container->addSectionContent(p_SchemaContent, NULL, ADS_NS::LeftDropArea);



    // =============== Enable DockWidget ==============
    // setDockNestingEnabled(true);

    // QDockWidget* p_schemaDockWidget = new QDockWidget(this);
    // p_schemaDockWidget->setWidget(p_schemaWiddget);
    // p_schemaDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    // addDockWidget(Qt::LeftDockWidgetArea, p_schemaDockWidget);

    // QDockWidget* p_editorDockWidget = new QDockWidget(this);
    // p_editorDockWidget->setWidget(p_editorWidget);
    // p_editorDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    // addDockWidget(Qt::LeftDockWidgetArea, p_editorDockWidget);

    // QDockWidget* p_attributeDockWidget = new QDockWidget(this);
    // p_attributeDockWidget->setWidget(p_attributeArea);
    // p_attributeDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    // addDockWidget(Qt::LeftDockWidgetArea, p_attributeDockWidget);

    // QDockWidget* p_auxiliaryDockWidget = new QDockWidget(this);
    // p_auxiliaryDockWidget->setWidget(p_auxiliaryArea);
    // p_auxiliaryDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    // addDockWidget(Qt::LeftDockWidgetArea, p_auxiliaryDockWidget);

    // // 设置布局
    // addDockWidget(Qt::LeftDockWidgetArea, p_schemaDockWidget);
    // splitDockWidget(p_schemaDockWidget, p_editorDockWidget, Qt::Horizontal);
    // splitDockWidget(p_editorDockWidget, p_attributeDockWidget, Qt::Horizontal);
    // splitDockWidget(p_editorDockWidget, p_auxiliaryDockWidget, Qt::Vertical);

    // // 设置dock的初始大小
    // resizeDocks({ p_schemaDockWidget }, { 150 }, Qt::Horizontal);
    // resizeDocks({ p_attributeDockWidget }, { 150 }, Qt::Horizontal);
    // resizeDocks({ p_auxiliaryDockWidget }, { 150 }, Qt::Vertical);
    // // 删除centralwidget，所有部件均为dockwidget
    // QWidget* p = takeCentralWidget();
    // if (p)
    //     delete p;


    // =============== Disable DockWidget ==============
    // 设置布局
    QSplitter* vSplitter = new QSplitter(Qt::Vertical, this);
    vSplitter->addWidget(p_editorWidget);
    vSplitter->addWidget(p_auxiliaryArea);

    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(p_schemaWiddget);
    splitter->addWidget(vSplitter);
    splitter->addWidget(p_attributeArea);

    vSplitter->setHandleWidth(0);
    splitter->setHandleWidth(0);

    this->setCentralWidget(splitter);

    // 设置dock的初始大小
    splitter->setSizes(QList<int>({ 150, 300, 150, 150 }));
}


void MainWindow::setupNoDockingLayout()
{
}

// 设置菜单栏样式
void MainWindow::setupMenuBar()
{
    p_menuBar = new GLDMenuBar(this);
    this->setMenuBar(p_menuBar);

    refresh = new QAction(this);
    refresh->setText("Refresh");

    connect(refresh, &QAction::triggered, this, [this]() {
        QFile file("assets/themes/dark/stylesheet/dark.qss");

        if (file.open(QFile::ReadOnly))
        {
            QString style = file.readAll();
            qApp->setStyleSheet(style);
        }
    });


    // TODO: Icons Download

    connect(p_menuBar->actionOpen, &QAction::triggered, this, [this]() { p_editorWidget->openFile(); });
    connect(p_menuBar->actionSave, &QAction::triggered, this, [this]() { p_editorWidget->saveFile(); });
    connect(p_menuBar->actionSaveAs, &QAction::triggered, this, [this]() { p_editorWidget->saveAs(); });

    connect(p_menuBar->actionCut, &QAction::triggered, this, [this]() { p_editorWidget->cut(); });
    connect(p_menuBar->actionPaste, &QAction::triggered, this, [this]() { p_editorWidget->paste(); });
    connect(p_menuBar->actionCopy, &QAction::triggered, this, [this]() { p_editorWidget->copy(); });

    connect(p_menuBar->actionRedo, &QAction::triggered, this, [this]() { p_editorWidget->redo(); });
    connect(p_menuBar->actionUndo, &QAction::triggered, this, [this]() { p_editorWidget->undo(); });

    connect(p_menuBar->actionExit, &QAction::triggered, this, [this]() { QApplication::exit(0); });

    connect(p_menuBar->actionAbout, &QAction::triggered, this, &MainWindow::showAbout);
    connect(p_menuBar->actionHelp, &QAction::triggered, this, &MainWindow::showHelp);

    p_menuBar->actionNew->setShortcut(QKeySequence("Ctrl+N"));
    p_menuBar->actionOpen->setShortcut(QKeySequence("Ctrl+O"));
    p_menuBar->actionSave->setShortcut(QKeySequence("Ctrl+S"));
    p_menuBar->actionSaveAs->setShortcut(QKeySequence("Ctrl+Shift+S"));
    p_menuBar->actionExit->setShortcut(QKeySequence("Ctrl+Q"));
    p_menuBar->actionCut->setShortcut(QKeySequence("Ctrl+X"));
    p_menuBar->actionCopy->setShortcut(QKeySequence("Ctrl+C"));
    p_menuBar->actionPaste->setShortcut(QKeySequence("Ctrl+V"));
    p_menuBar->actionFind->setShortcut(QKeySequence("Ctrl+F"));
    p_menuBar->actionFindNext->setShortcut(QKeySequence("Ctrl+Shift+F"));
    p_menuBar->actionReplace->setShortcut(QKeySequence("Ctrl+R"));

    // 可见性
    connect(p_menuBar->actionAttribute, &QAction::triggered, this, [this]() {
        if (!p_attributeArea->isHidden())
            p_attributeArea->hide();
        else
            p_attributeArea->show();
    });
    connect(p_menuBar->actionSchema, &QAction::triggered, this, [this]() {
        if (!p_schemaWiddget->isHidden())
            p_schemaWiddget->hide();
        else
            p_schemaWiddget->show();
    });
    connect(p_menuBar->actionAuxiliary, &QAction::triggered, this, [this]() {
        if (!p_auxiliaryArea->isHidden())
            p_auxiliaryArea->hide();
        else
            p_auxiliaryArea->show();
    });
    connect(p_menuBar->actionStatusbar, &QAction::triggered, this, [this]() {
        if (!p_StatusBar->isHidden())
            p_StatusBar->hide();
        else
            p_StatusBar->show();
    });
    connect(p_menuBar->actionToolbar, &QAction::triggered, this, [this]() {
        if (!p_toolBar->isHidden())
            p_toolBar->hide();
        else
            p_toolBar->show();
    });

    // theme
    // connect(p_menuBar->actionLight, &QAction::triggered, this, [this]() { p_toolBar->setLightIcon(); });
    // connect(p_menuBar->actionDark, &QAction::triggered, this, [this]() { p_toolBar->setDarkIcon(); });

    // Font
    connect(p_menuBar, &GLDMenuBar::signalSetFontFamily, this, [this](const QString& font) { p_editorWidget->setEditorFontFamily(font); });
    connect(p_menuBar, &GLDMenuBar::signalSetFontSize, this, [this](int fontSize) { p_editorWidget->setFontSize(fontSize); });
}

void MainWindow::setupToolBar()
{
    p_toolBar = new GLDToolBar();
    this->addToolBar(p_toolBar);
    connect(this->p_toolBar->p_new, &QAction::triggered, this, [this]() { p_editorWidget->newFile(); });
    connect(this->p_toolBar->p_open, &QAction::triggered, this, [this]() { p_editorWidget->openFile(); });

    connect(this->p_toolBar->p_save, &QAction::triggered, this, [this]() { p_editorWidget->saveFile(); });
    connect(this->p_toolBar->p_cut, &QAction::triggered, this, [this]() { p_editorWidget->cut(); });
    connect(this->p_toolBar->p_copy, &QAction::triggered, this, [this]() { p_editorWidget->copy(); });
    connect(this->p_toolBar->p_paste, &QAction::triggered, this, [this]() { p_editorWidget->paste(); });
    connect(this->p_toolBar->p_redo, &QAction::triggered, this, [this]() { p_editorWidget->redo(); });
    connect(this->p_toolBar->p_undo, &QAction::triggered, this, [this]() { p_editorWidget->undo(); });

    // TODO
    connect(this->p_toolBar->p_find, &QAction::triggered, this, [this]() {});
    connect(this->p_toolBar->p_replace, &QAction::triggered, this, [this]() {});

    connect(p_editorWidget, &GLDEditorWidget::signalUpdateRecentFiles, this, [this](QQueue<QString>& recentFiles) {
        p_menuBar->menuRecent->clear();
        for (const auto& it : recentFiles)
        {
            QAction* action = new QAction(it, this);
            p_menuBar->menuRecent->addAction(action);
        }
    });

    connect(this->p_toolBar->p_refresh, &QAction::triggered, this, [this]() {
        QFile file("assets/themes/dark/stylesheet/dark.qss");
        if (file.open(QFile::ReadOnly))
        {
            QString style = file.readAll();
            qApp->setStyleSheet(style);
        }
    });
}

MainWindow::~MainWindow()
{
}

void MainWindow::showAbout()
{
    // 创建一个QMessageBox对象，显示About相关信息
    QMessageBox messageAbout;
    messageAbout.setWindowTitle(QStringLiteral("关于"));
    messageAbout.setText(QStringLiteral("<h2>C++大作业</h2>"
                                        "<p>版权所有 © 20240830 [TOT24 C++班 第五组]。</p>"));

    messageAbout.setIcon(QMessageBox::Information);                   // 先移除默认图标
    messageAbout.setWindowIcon(QIcon(QString(":/image/about.png")));  // 设置自定义图标
    messageAbout.exec();
}

void MainWindow::showHelp()
{
    // 创建一个QMessageBox对象，显示About相关信息
    QMessageBox messageAbout;
    messageAbout.setWindowTitle(QStringLiteral("Help"));
    messageAbout.setText(QStringLiteral("Only yourself can help you.\t\t\nOr ask for the God!"));

    messageAbout.setIcon(QMessageBox::Warning);  // 先移除默认图标
    messageAbout.exec();
}

void MainWindow::setupAuxiliary()
{
    connect(p_auxiliaryArea->m_searchWindow, &GLDSearchWindow::signalNextSearch, this,
            [this](const QString& searchText, const bool caseSensitive, const bool wholeWord) {
                p_editorWidget->findAllText(searchText, caseSensitive, wholeWord);
                p_editorWidget->findNextText();
                if (p_editorWidget->isNewSearch())
                    p_auxiliaryArea->setSearchResults(p_editorWidget->getMatchedList());
            });

    connect(p_auxiliaryArea->m_searchWindow, &GLDSearchWindow::signalPrevSearch, this,
            [this](const QString& searchText, const bool caseSensitive, const bool wholeWord) {
                p_editorWidget->findAllText(searchText, caseSensitive, wholeWord);
                p_editorWidget->findPreviousText();
                if (p_editorWidget->isNewSearch())
                    p_auxiliaryArea->setSearchResults(p_editorWidget->getMatchedList());
            });

    connect(p_auxiliaryArea->m_searchWindow, &GLDSearchWindow::signalSearchTextChanged, this,
            [this](const QString& text, const bool caseSensitive, const bool wholeWord) {
                p_editorWidget->findAllText(text, caseSensitive, wholeWord);
                p_auxiliaryArea->setSearchResults(p_editorWidget->getMatchedList());
            });

    connect(p_auxiliaryArea->m_searchWindow, &GLDSearchWindow::signalReplaceTextChanged, this,
            [this](const QString& text, const bool caseSensitive, const bool wholeWord) {
                p_editorWidget->findAllText(text, caseSensitive, wholeWord);
                p_auxiliaryArea->setSearchResults(p_editorWidget->getMatchedList());
            });

    connect(p_menuBar->actionFind, &QAction::triggered, this, [this]() { p_auxiliaryArea->show(); });
    connect(p_menuBar->actionFindNext, &QAction::triggered, this, [this]() { p_auxiliaryArea->show(); });
    connect(p_menuBar->actionReplace, &QAction::triggered, this, [this]() { p_auxiliaryArea->show(); });
}


QWidget* MainWindow::GetMainWindow()
{
    QWidget* parent = dynamic_cast<QWidget*>(this->parent());
    if (parent)
        return parent;
    else
        return this;
}

CustomWindow* MainWindow::GetCustomWindow()
{
    CustomWindow* win = dynamic_cast<CustomWindow*>(GetMainWindow());

    if (win)
        return win;
    else
        return nullptr;
}