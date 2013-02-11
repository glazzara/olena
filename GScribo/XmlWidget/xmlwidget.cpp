#include "xmlwidget.h"

XmlWidget::XmlWidget(QWidget *parent) :
        QWidget(parent)
{
    QHBoxLayout *hLayout = new QHBoxLayout;

    proxy_.setFilterRegExp(QRegExp("Page|.Region|Line", Qt::CaseInsensitive));
    proxy_.setFilterRole(Qt::UserRole);
    proxy_.setSourceModel(&model_);
    proxy_.setDynamicSortFilter(true);

    selectionProxy_.setFilterRole(Qt::UserRole+1);
    selectionProxy_.setSourceModel(&proxy_);
    selectionProxy_.setDynamicSortFilter(true);

    selection_.setModel(&selectionProxy_);
    selection_.setUniformRowHeights(true);
    selection_.setItemDelegate(new XmlDelegate(Qt::UserRole));

    property_.setModel(&attributesModel_);
    property_.setUniformRowHeights(true);
    property_.setItemsExpandable(false);
    property_.setColumnWidth(0, property_.width() / 2.5);
    property_.setSelectionMode(QAbstractItemView::NoSelection);
    property_.setItemDelegateForColumn(0, new XmlDelegate(Qt::UserRole));
    property_.setItemDelegateForColumn(1, new XmlDelegate(Qt::UserRole+1));

    hLayout->addWidget(&selection_);
    hLayout->addWidget(&property_);

    setLayout(hLayout);

    connect(&selection_, SIGNAL(clicked(QModelIndex)), this, SLOT(onClick(QModelIndex)));
}

void XmlWidget::changeView(XmlItem *rootItem)
{
    model_.load(rootItem);
}

void XmlWidget::onClick(const QModelIndex& index)
{
    attributesModel_.load(index.data(Qt::UserRole+2).value<XmlAttributes>());
    PolygonItem *graphicalItem = VariantPointer<PolygonItem>::fromQVariant(index.data(Qt::UserRole+3));

    if(graphicalItem)
        emit select(graphicalItem);
}
