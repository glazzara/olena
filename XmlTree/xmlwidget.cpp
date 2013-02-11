#include "xmlwidget.h"

XmlWidget::XmlWidget(QTreeWidgetItem *item)
{
    tree.setParent(this);
    load(item);
}
