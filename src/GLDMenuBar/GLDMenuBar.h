#include <QMenu>
#include <QWidget>
#include <QMenuBar>
#include <QAction>

class MainWindow;

class GLDMenuBar : public QMenuBar
{
    Q_OBJECT

    friend MainWindow;

  public:
    explicit GLDMenuBar(QWidget* parent);
    ~GLDMenuBar();

  private:
    void setLightIcon();
    void setDarkIcon();

    // ======= Menu ========
    QMenu* menuFile;
    QMenu* menuEdit;
    QMenu* menuNavigation;
    QMenu* menuTools;
    QMenu* menuHelp;

    // ======== Actions =========
    // File
    QAction* actionNew;
    QAction* actionOpen;
    QAction* actionSave;
    QAction* actionSaveAs;
    QMenu*   menuRecent;
    QAction* actionExit;
    QAction* actionNone;
    // Edit
    QAction* actionUndo;
    QAction* actionRedo;
    QAction* actionCut;
    QAction* actionCopy;
    QAction* actionPaste;
    QAction* actionFind;
    QAction* actionFindNext;
    QAction* actionReplace;

    // Navigation
    QAction* actionForward;
    QAction* actionBackword;
    QAction* actionLocation;

    // Tools
    QAction* actionColor;
    QAction* actionCite;
    QAction* actionCheck;
    QMenu*   menuView;
    QAction* actionAuxiliary;
    QAction* actionAttribute;
    QAction* actionSchema;
    QAction* actionToolbar;
    QAction* actionStatusbar;

    QMenu*   menuTheme;
    QAction* actionLight;
    QAction* actionDark;

    // Help
    QAction* actionHelp;
    QAction* actionAbout;
};