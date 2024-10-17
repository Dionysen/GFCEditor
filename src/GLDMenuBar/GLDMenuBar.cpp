#include "GLDMenuBar.h"
#include "GLDStyleManager.h"

#include <QFile>
#include <QApplication>
#include <QFontDialog>
#include <QInputDialog>


GLDMenuBar::GLDMenuBar(QWidget* parent)
    : QMenuBar(parent)
{
    menuFile       = new QMenu("File", this);
    menuEdit       = new QMenu("Edit", this);
    menuNavigation = new QMenu("Navigate", this);
    menuTools      = new QMenu("Tools", this);
    menuHelp       = new QMenu("Help", this);

    actionNew    = new QAction("New File", this);
    actionOpen   = new QAction("Open File", this);
    actionSave   = new QAction("Save", this);
    actionSaveAs = new QAction("Save As", this);
    menuRecent   = new QMenu("Recent Files", this);
    actionExit   = new QAction("Exit", this);
    actionNone   = new QAction("None", this);

    actionUndo     = new QAction("Undo", this);
    actionRedo     = new QAction("Redo", this);
    actionCut      = new QAction("Cut", this);
    actionCopy     = new QAction("Copy", this);
    actionPaste    = new QAction("Paste", this);
    actionFind     = new QAction("Find", this);
    actionFindNext = new QAction("Find Next", this);
    actionReplace  = new QAction("Replace", this);

    actionForward  = new QAction("Forward", this);
    actionBackword = new QAction("Backword", this);
    actionLocation = new QAction("Loate", this);

    actionColor      = new QAction("Font Color", this);
    actionCite       = new QAction("Cited Entity", this);
    actionCheck      = new QAction("Legitimacy Check", this);
    menuView         = new QMenu("View", this);
    actionAuxiliary  = new QAction("Auxiliary", this);
    actionAttribute  = new QAction("Attribute", this);
    actionSchema     = new QAction("Schema", this);
    actionToolbar    = new QAction("Tool Bar", this);
    actionStatusbar  = new QAction("Status Bar", this);
    actionFontFamily = new QAction("Font Family", this);
    actionFontSize   = new QAction("Font Size", this);
    menuFont         = new QMenu("Font", this);

    menuTheme   = new QMenu("Theme", this);
    actionLight = new QAction("Light", this);
    actionDark  = new QAction("Dark", this);

    actionHelp  = new QAction("Help", this);
    actionAbout = new QAction("About", this);

    this->addMenu(menuFile);
    this->addMenu(menuEdit);
    this->addMenu(menuNavigation);
    this->addMenu(menuTools);
    this->addMenu(menuHelp);

    menuFile->addAction(actionNew);
    menuFile->addAction(actionOpen);
    menuFile->addSeparator();
    menuFile->addAction(actionSave);
    menuFile->addAction(actionSaveAs);
    menuFile->addSeparator();
    menuFile->addMenu(menuRecent);
    menuRecent->addAction(actionNone);
    menuFile->addSeparator();
    menuFile->addAction(actionExit);

    menuEdit->addAction(actionUndo);
    menuEdit->addAction(actionRedo);
    menuEdit->addSeparator();
    menuEdit->addAction(actionCut);
    menuEdit->addAction(actionCopy);
    menuEdit->addAction(actionPaste);
    menuEdit->addSeparator();
    menuEdit->addAction(actionFind);
    menuEdit->addAction(actionFindNext);
    menuEdit->addAction(actionReplace);

    menuNavigation->addAction(actionForward);
    menuNavigation->addAction(actionBackword);
    menuNavigation->addAction(actionLocation);

    menuTools->addAction(actionColor);
    menuTools->addAction(actionCite);
    menuTools->addAction(actionCheck);
    menuTools->addMenu(menuView);
    menuView->addAction(actionAuxiliary);
    menuView->addAction(actionAttribute);
    menuView->addAction(actionSchema);
    menuView->addAction(actionToolbar);
    menuView->addAction(actionStatusbar);
    menuTools->addMenu(menuTheme);
    menuTheme->addAction(actionLight);
    menuTheme->addAction(actionDark);
    menuTools->addSeparator();
    menuTools->addMenu(menuFont);
    menuFont->addAction(actionFontFamily);
    menuFont->addAction(actionFontSize);

    menuHelp->addAction(actionHelp);
    menuHelp->addAction(actionAbout);

    connect(actionLight, &QAction::triggered, this, [this]() {  // 设置样式
        QFile file(":/qss/light.qss");
        if (file.open(QFile::ReadOnly))
        {
            QString style = file.readAll();
            qApp->setStyleSheet(style);
        }
        GLDStyleManager::StyleManager()->setTheme("light");
    });
    connect(actionDark, &QAction::triggered, this, [this]() {
        QFile file(":/qss/dark.qss");
        if (file.open(QFile::ReadOnly))
        {
            QString style = file.readAll();
            qApp->setStyleSheet(style);
        }
        GLDStyleManager::StyleManager()->setTheme("dark");
    });

    connect(actionFontFamily, &QAction::triggered, this, [this]() {
        bool  ok;
        QFont font = QFontDialog::getFont(&ok, this);
        if (ok)
        {
            emit signalSetFontFamily(font.family());
        }
    });

    connect(actionFontSize, &QAction::triggered, this, [this]() {
        bool ok;
        int  size = QInputDialog::getInt(this, "设置字体大小", "字体大小:", 12, 1, 100, 1, &ok);
        if (ok)
        {
            emit signalSetFontSize(size);
        }
    });

    connect(GLDStyleManager::StyleManager(), &GLDStyleManager::themeChanged, this, &GLDMenuBar::setIcon);
    setIcon();
}

void GLDMenuBar::setIcon()
{
    actionNew->setIcon(GLDStyleManager::StyleManager()->getIcon("new"));
    actionOpen->setIcon(GLDStyleManager::StyleManager()->getIcon("open"));
    actionSave->setIcon(GLDStyleManager::StyleManager()->getIcon("save"));
    actionSaveAs->setIcon(GLDStyleManager::StyleManager()->getIcon("saveAs"));
    actionExit->setIcon(GLDStyleManager::StyleManager()->getIcon("exit"));
    actionSaveAs->setIcon(GLDStyleManager::StyleManager()->getIcon("new"));
    actionExit->setIcon(GLDStyleManager::StyleManager()->getIcon("new"));
    actionRedo->setIcon(GLDStyleManager::StyleManager()->getIcon("redo"));
    actionUndo->setIcon(GLDStyleManager::StyleManager()->getIcon("undo"));
    actionCut->setIcon(GLDStyleManager::StyleManager()->getIcon("cut"));
    actionCopy->setIcon(GLDStyleManager::StyleManager()->getIcon("copy"));
    actionPaste->setIcon(GLDStyleManager::StyleManager()->getIcon("paste"));
    actionFind->setIcon(GLDStyleManager::StyleManager()->getIcon("find"));
    actionFindNext->setIcon(GLDStyleManager::StyleManager()->getIcon("findNext"));
    actionReplace->setIcon(GLDStyleManager::StyleManager()->getIcon("replace"));
    actionBackword->setIcon(GLDStyleManager::StyleManager()->getIcon("backward"));
    actionForward->setIcon(GLDStyleManager::StyleManager()->getIcon("forward"));
    actionLocation->setIcon(GLDStyleManager::StyleManager()->getIcon("location"));
    menuView->setIcon(GLDStyleManager::StyleManager()->getIcon("visible"));
    actionAuxiliary->setIcon(GLDStyleManager::StyleManager()->getIcon("light"));
    actionAttribute->setIcon(GLDStyleManager::StyleManager()->getIcon("light"));
    actionSchema->setIcon(GLDStyleManager::StyleManager()->getIcon("light"));
    actionToolbar->setIcon(GLDStyleManager::StyleManager()->getIcon("light"));
    actionStatusbar->setIcon(GLDStyleManager::StyleManager()->getIcon("light"));
    menuRecent->setIcon(GLDStyleManager::StyleManager()->getIcon("recent"));
    actionColor->setIcon(GLDStyleManager::StyleManager()->getIcon("color"));
    actionCite->setIcon(GLDStyleManager::StyleManager()->getIcon("cite"));
    actionCheck->setIcon(GLDStyleManager::StyleManager()->getIcon("check"));
    actionHelp->setIcon(GLDStyleManager::StyleManager()->getIcon("help"));
    actionAbout->setIcon(GLDStyleManager::StyleManager()->getIcon("about"));
    menuFont->setIcon(GLDStyleManager::StyleManager()->getIcon("font"));
}

GLDMenuBar::~GLDMenuBar()
{
}