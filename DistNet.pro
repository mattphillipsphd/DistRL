TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS_DEBUG += -DDN_DEBUG

SOURCES += main.cpp \
    state.cpp \
    chunk.cpp \
    rlbase.cpp \
    chunkmgr.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    globals.h \
    state.h \
    chunk.h \
    transprob.h \
    reward.h \
    rlbase.h \
    value.h \
    chunkmgr.h

