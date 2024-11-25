QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    database_manager.cpp \
    database_user.cpp \
    database_vocabulary.cpp \
    dialog_user_login.cpp \
    dialog_user_registration.cpp \
    main.cpp \
    question_base.cpp \
    question_fill_blank.cpp \
    resource_manager.cpp \
    window_main.cpp \
    window_word_card.cpp

HEADERS += \
    database_manager.h \
    database_user.h \
    database_vocabulary.h \
    dialog_user_login.h \
    dialog_user_registration.h \
    question_base.h \
    question_fill_blank.h \
    resource_manager.h \
    window_main.h \
    window_word_card.h

FORMS += \
    dialog_user_login.ui \
    dialog_user_registration.ui \
    question_fill_blank.ui \
    window_main.ui \
    window_word_card.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    picture.qrc
