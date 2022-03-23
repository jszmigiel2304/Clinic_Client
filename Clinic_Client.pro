#-------------------------------------------------
#
# Project created by QtCreator 2018-03-19T16:16:26
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Clinic_Client
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    c_connectiontoservercontroller.cpp \
    c_loggeduser.cpp \
    c_logscontroller.cpp \
    c_myfiles.cpp \
    c_parser.cpp \
    c_sessioncontroller.cpp \
    c_threadcontroller.cpp \
    m_loggeduser.cpp \
    m_module.cpp \
        main.cpp \
    w_logswindow.cpp \
    w_mainwindow.cpp \
    i_observer.cpp \
    i_watched.cpp \
    c_settingscontroller.cpp \
    w_modulebuttonbig.cpp \
    w_modulebuttonlabel.cpp \
    w_modulebuttonsmall.cpp \
    w_serverconnectiondialog.cpp \
    c_clinicclient.cpp \
    c_versioncontroller.cpp \
    c_apptrayicon.cpp \
    w_loggingdialog.cpp \
    c_myconnectiontester.cpp \
    w_modulesscrollarea.cpp \
    w_modulebutton.cpp \
    c_modulescontroller.cpp \
    c_module.cpp \
    w_informationslabel.cpp \
    c_modulebuttonsfactory.cpp \
    w_toppanel.cpp \
    w_userpanel.cpp

HEADERS += \
    c_connectiontoservercontroller.h \
    c_loggeduser.h \
    c_logscontroller.h \
    c_myfiles.h \
    c_parser.h \
    c_sessioncontroller.h \
    c_threadcontroller.h \
    m_loggeduser.h \
    m_module.h \
    w_logswindow.h \
    w_mainwindow.h \
    i_observer.h \
    i_watched.h \
    c_settingscontroller.h \
    m_defaultinifile.h \
    w_modulebuttonbig.h \
    w_modulebuttonlabel.h \
    w_modulebuttonsmall.h \
    w_serverconnectiondialog.h \
    c_clinicclient.h \
    c_versioncontroller.h \
    c_apptrayicon.h \
    w_loggingdialog.h \
    c_myconnectiontester.h \
    w_modulesscrollarea.h \
    w_modulebutton.h \
    c_modulescontroller.h \
    c_module.h \
    c_mystructures.h \
    w_informationslabel.h \
    c_modulebuttonsfactory.h \
    w_toppanel.h \
    w_userpanel.h

FORMS += \
    w_logswindow.ui \
    w_mainwindow.ui \
    w_serverconnectiondialog.ui \
    w_loggingdialog.ui

DISTFILES += \
    config.ini

RESOURCES += \
    icons.qrc

RC_ICONS += Clinic_Client.ico

