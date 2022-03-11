QT  += core gui sql

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
    dboperator.cpp \
    main.cpp \
    mainwidget.cpp \
    noregisteredform.cpp \
    passedform.cpp \
    registerform.cpp \
    searchform.cpp \
    toast.cpp \
    user.cpp \
    userlistform.cpp \
    warnform.cpp \
    workingwidget.cpp

HEADERS += \
    dboperator.h \
    mainwidget.h \
    noregisteredform.h \
    passedform.h \
    registerform.h \
    searchform.h \
    toast.h \
    user.h \
    userlistform.h \
    warnform.h \
    workingwidget.h

FORMS += \
    mainwidget.ui \
    noregisteredform.ui \
    passedform.ui \
    registerform.ui \
    searchform.ui \
    userlistform.ui \
    warnform.ui \
    workingwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    plamres.qrc