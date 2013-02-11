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
    xml.cpp \
    Rendering/scene.cpp \
    Rendering/selection.cpp \
    Rendering/polygonitem.cpp \
    PagesWidget/pageswidget.cpp \
    PagesWidget/listmodel.cpp \
    Preferences/segmentationoptions.cpp \
    Preferences/preprocessingoptions.cpp \
    Preferences/ocroptions.cpp \
    Preferences/generaloptions.cpp \
    Preferences/preferencesdialog.cpp \
    XmlWidget/xmlwidget.cpp \
    XmlWidget/xmlmodel.cpp \
    XmlWidget/xmlitem.cpp \
    XmlWidget/xmlattributes.cpp \
    XmlWidget/xmlview.cpp
HEADERS += mainwindow.h \
    variantpointer.h \
    configs.h \
    region.h \
    dir.h \
    xml.h \
    Rendering/scene.h \
    Rendering/selection.h \
    Rendering/polygonitem.h \
    PagesWidget/pageswidget.h \
    PagesWidget/listmodel.h \
    Preferences/segmentationoptions.h \
    Preferences/preprocessingoptions.h \
    Preferences/ocroptions.h \
    Preferences/generaloptions.h \
    Preferences/optionwidget.h \
    Preferences/preferencesdialog.h \
    XmlWidget/xmlwidget.h \
    XmlWidget/xmlmodel.h \
    XmlWidget/xmlitem.h \
    XmlWidget/xmlattributes.h \
    XmlWidget/treedelegate.h \
    XmlWidget/attributesdelegate.h \
    XmlWidget/xmlview.h
FORMS += mainwindow.ui \
    Preferences/preferencesdialog.ui \
    Preferences/ocroptions.ui \
    Preferences/segmentationoptions.ui \
    Preferences/generaloptions.ui \
    Preferences/preprocessingoptions.ui
