QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    database_manager.cpp \
    main.cpp \
    mainwindow.cpp \
    memorytestbyfillrandomblank.cpp \
    userlogin.cpp \
    userregistration.cpp \
    vocabularydatabase.cpp \
    WordCard.cpp

HEADERS += \
    database_manager.h \
    mainwindow.h \
    memorytestbyfillrandomblank.h \
    userlogin.h \
    userregistration.h \
    vocabularydatabase.h \
    WordCard.h

FORMS += \
    mainwindow.ui \
    memorytestbyfillrandomblank.ui \
    userlogin.ui \
    userregistration.ui \
    WordCard.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    picture.qrc
