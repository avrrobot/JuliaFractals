#-------------------------------------------------
#
# Project created by QtCreator 2014-09-30T18:04:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Julia_Fractals
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11 \
                    -O3 \
                    -fexpensive-optimizations

QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp

SOURCES += main.cpp\
        mainwindow.cpp \
    fractalrenderer.cpp \
    movablewindow.cpp

HEADERS  += mainwindow.h \
    complex.h \
    fractalrenderer.h \
    movablewindow.h

FORMS    += mainwindow.ui \
    movablewindow.ui
