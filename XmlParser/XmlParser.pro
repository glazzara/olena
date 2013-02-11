# -------------------------------------------------
# Project created by QtCreator 2013-01-11T14:21:43
# -------------------------------------------------
QT += xml
TARGET = XmlParser
TEMPLATE = app
INCLUDEPATH += /lrde/home/stage/froger_a/olena/milena/ \
    /lrde/home/stage/froger_a/olena/scribo/
QMAKE_CXXFLAGS += -DNDEBUG \
    -DMLN_WO_GLOBAL_VARS
LIBS += -I/usr/include/graphicsImage \
    -lGraphicsMagick++ \
    -ltesseract_full
SOURCES += main.cpp \
    mainwindow.cpp \
    xml.cpp \
    scene.cpp \
    selection.cpp \
    polygonitem.cpp \
    runner.cpp \
    toolbar.cpp \
    process.cpp \
    preprocess.cpp \
    configs.cpp \
    progressdialog.cpp
HEADERS += mainwindow.h \
    region.h \
    xml.h \
    scene.h \
    selection.h \
    polygonitem.h \
    runner.h \
    toolbar.h \
    demodir.h \
    process.h \
    preprocess.h \
    configs.h \
    dir.h \
    progressdialog.h
FORMS += mainwindow.ui
