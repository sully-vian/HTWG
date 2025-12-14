TEMPLATE    = app
TARGET      = RangeQueries
DESTDIR     = ./debug
QT          += core opengl widgets gui  openglwidgets
CONFIG      += debug c++20
DEFINES     += QT_DLL QT_DLL QT_OPENGL_LIB QT_WIDGETS_LIB
INCLUDEPATH +=  ./GeneratedFiles \
                . \
                ./GeneratedFiles/Debug \
                ./external/freeglut/include
LIBS        +=  -L./external/freeglut/lib/x64   # on Linux and Mac use "LIBS += -lglut" instead
LIBS        +=  -lGLU
DEPENDPATH  += .
MOC_DIR     += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR      +=  ./GeneratedFiles
RCC_DIR     +=  ./GeneratedFiles
HEADERS     +=  ./mainwindow.h \
                ./glwidget.h
SOURCES     +=  ./glwidget.cpp \
                ./main.cpp \
                ./mainwindow.cpp
FORMS       +=   ./mainwindow.ui
