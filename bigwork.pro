#-------------------------------------------------
#
# Project created by QtCreator 2024-08-29T10:38:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bigwork
TEMPLATE = app

INCLUDEPATH += $$PWD/GLDToolbar
INCLUDEPATH += $$PWD/GLDSchema
INCLUDEPATH += $$PWD/GLDEditor
INCLUDEPATH += $$PWD/GLDAuxiliaryArea
INCLUDEPATH += $$PWD/GLDAttributeArea
INCLUDEPATH += $$PWD/GLDEntity
INCLUDEPATH += $$PWD/GLDAttributeArea


SOURCES += main.cpp\
        MainWindow.cpp \
    GLDToolBar/GLDToolBar.cpp \
    GLDSchema/GLDSchemaWidget.cpp \
    GLDSchema/GLDTreeList.cpp \
    GLDEditor/GLDEditorWidget.cpp \
    GLDAttributeArea/GLDAttributeArea.cpp \
    GLDAuxiliaryArea/GLDAuxiliaryArea.cpp \
    GLDAuxiliaryArea/GLDErrorWindow.cpp \
    GLDAuxiliaryArea/GLDSearchWindow.cpp \
    GLDEntity/GLDEntity.cpp \
    GLDAttributeArea/GLDAttributeModel.cpp

HEADERS  += MainWindow.h \
    GLDToolBar/GLDToolBar.h \
    GLDSchema/GLDSchemaWidget.h \
    GLDSchema/GLDTreeList.h \
    GLDEditor/GLDEditorWidget.h \
    GLDAttributeArea/GLDAttributeArea.h \
    GLDAuxiliaryArea/GLDAuxiliaryArea.h \
    GLDAuxiliaryArea/GLDErrorWindow.h \
    GLDAuxiliaryArea/GLDSearchWindow.h \
    GLDEntity/GLDEntity.h \
    GLDAttributeArea/GLDAttributeModel.h

FORMS    += MainWindow.ui

RESOURCES += \
    assets/ToolbarIcon/img.qrc
