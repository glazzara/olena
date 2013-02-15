#include "xmlwidget.h"

XmlWidget::XmlWidget(QWidget *parent) :
        QSplitter(Qt::Horizontal, parent)
{
    proxy_.setFilterRegExp(QRegExp("(?:Page|.Region|Line)", Qt::CaseSensitive));
    proxy_.setDynamicSortFilter(true);
    proxy_.setFilterRole(Qt::UserRole);
    proxy_.setSourceModel(&model_);

    view_.setModel(&proxy_);

    attributes_.setModel(&attributesModel_);
    attributes_.setSelectionMode(QAbstractItemView::NoSelection);
    attributes_.setItemDelegateForColumn(0, new XmlDelegate(Qt::UserRole));
    attributes_.setItemDelegateForColumn(1, new XmlDelegate(Qt::UserRole+1));

    addWidget(&view_);
    addWidget(&attributes_);

    connect(&view_, SIGNAL(resetProperty()), &attributesModel_, SLOT());
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
