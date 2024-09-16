#include "GLDMenuBar.h"

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

    actionColor     = new QAction("Font Color", this);
    actionCite      = new QAction("Cited Entity", this);
    actionCheck     = new QAction("Legitimacy Check", this);
    menuView        = new QMenu("View", this);
    actionAuxiliary = new QAction("Auxiliary", this);
    actionAttribute = new QAction("Attribute", this);
    actionSchema    = new QAction("Schema", this);
    actionToolbar   = new QAction("Tool Bar", this);
    actionStatusbar = new QAction("Status Bar", this);

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

    menuHelp->addAction(actionHelp);
    menuHelp->addAction(actionAbout);
}
GLDMenuBar::~GLDMenuBar()
{
}