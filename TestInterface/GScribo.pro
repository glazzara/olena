# -------------------------------------------------
# Project created by QtCreator 2013-01-21T09:20:54
# -------------------------------------------------
QT += opengl \
    xml
INCLUDEPATH += /lrde/home/stage/froger_a/olena/milena/ \
    /lrde/home/stage/froger_a/olena/scribo/ \
    /lrde/home/stage/froger_a/olena/_build/scribo/demo/
QMAKE_CXXFLAGS += -DNDEBUG \
    -DMLN_WO_GLOBAL_VARS
LIBS += -I/usr/include/graphicsImage \
    -lGraphicsMagick++ \
    -ltesseract_full
TARGET = GScribo
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    scene.cpp \
    selection.cpp
HEADERS += mainwindow.h \
    scene.h \
    selection.h
FORMS += mainwindow.ui
