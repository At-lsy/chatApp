QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addfrienddlg.cpp \
    chatdlg.cpp \
    chatfileitemdlg.cpp \
    chatlistdlg.cpp \
    chatlistitemdlg.cpp \
    chattextitemdlg.cpp \
    contactsinfodlg.cpp \
    contactslistdlg.cpp \
    contactslistitemdlg.cpp \
    cutimagedlg.cpp \
    globals.cpp \
    main.cpp \
    logindlg.cpp \
    maindlg.cpp \
    mydatabase.cpp \
    mytcpsocket.cpp \
    newfrienddlg.cpp \
    newfrientitemdlg.cpp \
    registerdlg.cpp \
    searchfriendfaildlg.cpp \
    searchfriendlistdlg.cpp \
    searchfriendlistitemdlg.cpp \
    searchfriendsucceeddlg.cpp \
    selectheadimagedlg.cpp \
    sendfilesocket.cpp \
    winmove.cpp

HEADERS += \
    addfrienddlg.h \
    chatdlg.h \
    chatfileitemdlg.h \
    chatlistdlg.h \
    chatlistitemdlg.h \
    chattextitemdlg.h \
    contactsinfodlg.h \
    contactslistdlg.h \
    contactslistitemdlg.h \
    cutimagedlg.h \
    globals.h \
    logindlg.h \
    maindlg.h \
    mydatabase.h \
    mytcpsocket.h \
    newfrienddlg.h \
    newfrientitemdlg.h \
    registerdlg.h \
    searchfriendfaildlg.h \
    searchfriendlistdlg.h \
    searchfriendlistitemdlg.h \
    searchfriendsucceeddlg.h \
    selectheadimagedlg.h \
    sendfilesocket.h \
    winmove.h

FORMS += \
    addfrienddlg.ui \
    chatdlg.ui \
    chatfileitemdlg.ui \
    chatlistdlg.ui \
    chatlistitemdlg.ui \
    chattextitemdlg.ui \
    contactsinfodlg.ui \
    contactslistdlg.ui \
    contactslistitemdlg.ui \
    cutimagedlg.ui \
    logindlg.ui \
    maindlg.ui \
    newfrienddlg.ui \
    newfrientitemdlg.ui \
    registerdlg.ui \
    searchfriendfaildlg.ui \
    searchfriendlistdlg.ui \
    searchfriendlistitemdlg.ui \
    searchfriendsucceeddlg.ui \
    selectheadimagedlg.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src.qrc
