#include "xmlwidget.h"

XmlWidget::XmlWidget(QWidget *parent) :
        QSplitter(Qt::Horizontal, parent)
{
    proxy_.setFilterRegExp(QRegExp("Page|.Region|Line", Qt::CaseInsensitive));
    proxy_.setFilterRole(Qt::UserRole);
    proxy_.setSourceModel(&model_);
    proxy_.setDynamicSortFilter(true);

    view_.setModel(&proxy_);

    property_.setModel(&attributesModel_);
    property_.setUniformRowHeights(true);
    property_.setSelectionMode(QAbstractItemView::NoSelection);
    property_.setItemDelegateForColumn(0, new XmlDelegate(Qt::UserRole));
    property_.setItemDelegateForColumn(1, new XmlDelegate(Qt::UserRole+1));

    addWidget(&view_);
    addWidget(&property_);

    connect(&view_, SIGNAL(resetProperty()), &property_, SLOT(reset()));
    connect(&view_, SIGNAL(clicked(QModelIndex)), this, SLOT(loadAttributes(QModelIndex)));
    connect(&view_, SIGNAL(loadAttributes(XmlAttributes)), &attributesModel_, SLOT(load(XmlAttributes)));
}

void XmlWidget::changeView(XmlItem *rootItem)
{
    attributesModel_.reset();

    model_.load(rootItem);

    view_.displayAll();
    view_.expandAll();
}
