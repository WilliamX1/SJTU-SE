QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CompoundExp.cpp \
    ConstantExp.cpp \
    EndStmt.cpp \
    ErrorStmt.cpp \
    Expression.cpp \
    GotoStmt.cpp \
    IdentifierExp.cpp \
    IfStmt.cpp \
    InputStmt.cpp \
    InputsStmt.cpp \
    LetStmt.cpp \
    PrintStmt.cpp \
    PrintfStmt.cpp \
    RemStmt.cpp \
    Skiplist.cpp \
    Statement.cpp \
    StringExp.cpp \
    SyntaxTree.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    CompoundExp.h \
    ConstantExp.h \
    EndStmt.h \
    ErrorStmt.h \
    Expression.h \
    GotoStmt.h \
    IdentifierExp.h \
    IfStmt.h \
    InputStmt.h \
    InputsStmt.h \
    LetStmt.h \
    PrintStmt.h \
    PrintfStmt.h \
    RemStmt.h \
    Skiplist.h \
    Statement.h \
    StringExp.h \
    SyntaxTree.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
