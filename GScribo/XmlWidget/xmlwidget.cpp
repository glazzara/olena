#include "xmlwidget.h"

XmlWidget::XmlWidget(QWidget *parent) :
        QWidget(parent)
{
    QHBoxLayout *hLayout = new QHBoxLayout;

    proxy_.setFilterRegExp(QRegExp("Page|.Region|Line", Qt::CaseInsensitive));
    proxy_.setFilterRole(Qt::UserRole);
    proxy_.setSourceModel(&model_);
    proxy_.setDynamicSortFilter(true);

    view_.setModel(&proxy_);
    view_.setItemDelegate(new XmlDelegate(Qt::UserRole));

    property_.setModel(&attributesModel_);
    property_.setUniformRowHeights(true);
    property_.setColumnWidth(0, property_.width() / 2.5);
    property_.setSelectionMode(QAbstractItemView::NoSelection);
    property_.setItemDelegateForColumn(0, new XmlDelegate(Qt::UserRole));
    property_.setItemDelegateForColumn(1, new XmlDelegate(Qt::UserRole+1));

    hLayout->addWidget(&view_);
    hLayout->addWidget(&property_);

    setLayout(hLayout);

    connect(&view_, SIGNAL(resetAttributes()), &property_, SLOT(reset()));
}

void XmlWidget::changeView(XmlItem *rootItem)
{
    attributesModel_.reset();
    model_.load(rootItem);
    view_.expandAll();
}
