#include "xmlwidget.h"

XmlWidget::XmlWidget(XmlItem *item)
{
    XmlModel *model = new XmlModel(item);
    XmlDelegate *delegate = new XmlDelegate();

    proxy.setFilterRegExp(QRegExp(".Region|Line|Metadata|Page|Creator|Created|LastChange|Comments", Qt::CaseInsensitive));
    proxy.setFilterRole(Qt::UserRole);
    proxy.setSourceModel(model);

    tree.setParent(this);
    tree.setItemDelegate(delegate);
    tree.setModel(&proxy);
}
