#include "toolbar.h"

ToolBar::ToolBar(QWidget *parent):
        QToolBar("ToolBar", parent)
{
    setAutoFillBackground(true);
    setFloatable(false);
    setMovable(false);
    open = addAction(tr("Open"));
    segment = addAction(tr("Segment"));
    addSeparator();
    quit = addAction(tr("Quit"));
}

ToolBar::~ToolBar()
{
    delete open;
    delete segment;
    delete quit;
}

QAction * ToolBar::openAction() const
{
    return open;
}

QAction * ToolBar::segmentAction() const
{
    return segment;
}

QAction * ToolBar::quitAction() const
{
    return quit;
}
