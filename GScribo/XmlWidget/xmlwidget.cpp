#include "xmlwidget.h"

XmlWidget::XmlWidget(QWidget *parent) :
        QSplitter(Qt::Horizontal, parent)
{
    basePattern_ = "Page";
    proxy_.setFilterFixedString(basePattern_);
    proxy_.setFilterRole(Qt::UserRole);
    proxy_.setSourceModel(&model_);
    proxy_.setDynamicSortFilter(true);

    view_.setModel(&proxy_);

    attributes_.setModel(&attributesModel_);
    attributes_.setSelectionMode(QAbstractItemView::NoSelection);
    attributes_.setItemDelegateForColumn(0, new XmlDelegate(Qt::UserRole));
    attributes_.setItemDelegateForColumn(1, new XmlDelegate(Qt::UserRole+1));

    addWidget(&view_);
    addWidget(&attributes_);

    connect(&view_, SIGNAL(resetProperty()), &attributes_, SLOT(reset()));
    connect(&view_, SIGNAL(clicked(QModelIndex)), this, SLOT(loadAttributes(QModelIndex)));
    connect(&view_, SIGNAL(loadAttributes(XmlAttributes)), &attributesModel_, SLOT(load(XmlAttributes)));
}

void XmlWidget::setFilterString(const QString& filterString)
{
    proxy_.setFilterFixedString(basePattern_ + '|' + filterString);
}

void XmlWidget::changeView(XmlItem *rootItem)
{
    attributesModel_.reset();

    model_.load(rootItem);

    view_.displayAll();
    view_.expandAll();
}
