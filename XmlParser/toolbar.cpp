#include "toolbar.h"

ToolBar::ToolBar(QWidget *parent):
        QToolBar("ToolBar", parent)
{
    init();
}

void ToolBar::init()
{
    addAction(tr("Open"));
    addAction(tr("Segment"));

}
