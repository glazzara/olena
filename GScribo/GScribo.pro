# -------------------------------------------------
# Project created by QtCreator 2013-01-21T09:20:54
# -------------------------------------------------
QT += xml
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
    configs.cpp \
    Rendering/scene.cpp \
    Rendering/selection.cpp \
    Rendering/polygonitem.cpp \
    PagesViewer/pageswidget.cpp \
    PagesViewer/listmodel.cpp \
    Preferences/segmentationoptions.cpp \
    Preferences/preprocessingoptions.cpp \
    Preferences/ocroptions.cpp \
    Preferences/generaloptions.cpp \
    Preferences/optionwidget.cpp \
    Preferences/preferencesdialog.cpp \
    Processing/progressdialog.cpp \
    Processing/process.cpp \
    Processing/preprocess.cpp \
    Processing/runner.cpp \
    xml.cpp \
    xmlwidget.cpp
HEADERS += mainwindow.h \
    configs.h \
    region.h \
    Rendering/scene.h \
    Rendering/selection.h \
    Rendering/polygonitem.h \
    PagesViewer/pageswidget.h \
    PagesViewer/listmodel.h \
    Preferences/segmentationoptions.h \
    Preferences/preprocessingoptions.h \
    Preferences/ocroptions.h \
    Preferences/generaloptions.h \
    Preferences/optionwidget.h \
    Preferences/preferencesdialog.h \
    Processing/preprocess.h \
    Processing/progressdialog.h \
    Processing/process.h \
    dir.h \
    Processing/runner.h \
    xml.h \
    variantpointer.h \
    xmlwidget.h
FORMS += mainwindow.ui \
    Preferences/preferencesdialog.ui \
    Preferences/ocroptions.ui \
    Preferences/segmentationoptions.ui \
    Preferences/generaloptions.ui \
    Preferences/preprocessingoptions.ui
