#-------------------------------------------------
#
# Project created by QtCreator 2014-11-02T16:13:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = D0sag3Command
TEMPLATE = app


SOURCES += main.cpp\
        d3cwindow.cpp \
    D3CEncrypt.cpp \
    D3CHelp.cpp \
    D3CMath.cpp \
    D3CPrompt.cpp \
    D3CKinematics.cpp \
    D3CConvert.cpp \
    D3CConversation.cpp \
    D3CProgramConstants.cpp \
    D3CEncryptPW.cpp

HEADERS  += d3cwindow.h \
    D3CEncrypt.h \
    D3CHelp.h \
    D3CMath.h \
    D3CPrompt.h \
    D3CKinematics.h \
    D3CConvert.h \
    D3CConversation.h \
    D3CProgramConstants.h \
    D3CEncryptPW.h

FORMS    += d3cwindow.ui

OTHER_FILES += \
    ../build-D0sag3Command-Desktop_Qt_5_3_MinGW_32bit-Release/D3CCL.txt \
    ../build-D0sag3Command-Desktop_Qt_5_3_MinGW_32bit-Release/PrimeNumberList.txt
