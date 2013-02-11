#include "xmlwidget.h"

XmlWidget::XmlWidget(QWidget *parent) :
        QWidget(parent)
{
    QHBoxLayout *hLayout = new QHBoxLayout;

    proxy_.setFilterRegExp(QRegExp(".Region|Line|Metadata|Page|Creator|Created|LastChange|Comments", Qt::CaseInsensitive));
    proxy_.setFilterRole(Qt::UserRole);
    proxy_.setSourceModel(&model_);
    proxy_.setDynamicSortFilter(true);

    selectionProxy_.setFilterRole(Qt::UserRole);
    selectionProxy_.setSourceModel(&proxy_);
    selectionProxy_.setDynamicSortFilter(true);

    TreeDelegate *treeDelegate = new TreeDelegate;

    viewer_.setModel(&proxy_);
    viewer_.setItemDelegate(treeDelegate);

    selection_.setModel(&selectionProxy_);
    selection_.setItemDelegate(treeDelegate);

    hLayout->addWidget(&viewer_);
    hLayout->addWidget(&selection_);
    hLayout->addWidget(&property_);

    setLayout(hLayout);
}

void XmlWidget::changeView(XmlItem *rootItem)
{
    model_.load(rootItem);
}
