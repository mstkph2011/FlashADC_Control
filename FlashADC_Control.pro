#-------------------------------------------------
#
# Project created by QtCreator 2013-08-09T15:34:23
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FlashADC_Control
TEMPLATE = app

include ( /usr/local/qwt-6.1.3-svn//features/qwt.prf )
SOURCES += main.cpp\
				mainwindow.cpp \
		tcp_server.cpp \
		tcp_client.cpp \
		tcp_object.cpp \
    hv_control_window.cpp

HEADERS  += mainwindow.h \
		tcp_server.h \
		tcp_client.h \
		tcp_object.h \
    hv_control_window.h

FORMS    += \
		mainwindow.ui \
    hv_control_window.ui

unix|win32: LIBS += -lcaenhvwrapper

INCLUDEPATH +=/local/data/CAENHVWrapper-5.56/include


