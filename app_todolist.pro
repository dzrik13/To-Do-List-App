QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    add_task_window.cpp \
    edit_task_window.cpp \
    main.cpp \
    mainwindow.cpp \
    task_save.cpp \
    taskwidget.cpp

HEADERS += \
    add_task_window.h \
    edit_task_window.h \
    mainwindow.h \
    task_save.h \
    taskwidget.h

FORMS += \
    add_task_window.ui \
    edit_task_window.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
