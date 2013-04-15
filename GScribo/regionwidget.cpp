#include "regionwidget.h"

RegionWidget::RegionWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    setHeaderHidden(true);
    setSelectionMode(QAbstractItemView::NoSelection);

    addTopLevelItem(createRoot("Text", GraphicsRegion::Text, GraphicsRegion::TextRegion, GraphicsRegion::Line));
    addTopLevelItem(createRoot("Typological Lines", GraphicsRegion::Typology, GraphicsRegion::Baseline, GraphicsRegion::Meanline));
    addTopLevelItem(createRoot("Separators", GraphicsRegion::Separators, GraphicsRegion::VerticalSeparator, GraphicsRegion::WhiteSpaceSeparator));
    addTopLevelItem(createRoot("Miscellaneous", GraphicsRegion::Miscellaneous, GraphicsRegion::Image, GraphicsRegion::Chart));

    expandAll();

    connect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(checkStateChanged(QTreeWidgetItem*)));
}

QTreeWidgetItem *RegionWidget::createRoot(const QString& text, const GraphicsRegion::Id& region, const GraphicsRegion::Id& begin, const GraphicsRegion::Id& end)
{
    // Create root.
    QTreeWidgetItem *rootItem = createItem(text, region);

    // Fill root with corresponding childs.
    for(int region_ = begin; region_ <= end; region_++)
        fillRoot(rootItem, static_cast<GraphicsRegion::Id>(region_));

    return rootItem;
}

QTreeWidgetItem *RegionWidget::createItem(const QString& text, const GraphicsRegion::Id& region, const QColor& color)
{
    QTreeWidgetItem *item = new QTreeWidgetItem;
    item->setText(0, text);
    item->setCheckState(0, Qt::Checked);
    item->setBackgroundColor(0, color);
    // Store graphical id in the object to recognize it afterward.
    item->setData(0, Qt::UserRole, static_cast<int>(region));

    return item;
}

void RegionWidget::checkStateChanged(QTreeWidgetItem *item)
{
    // If it's a root item, go to childs.
    if(item->childCount() != 0)
    {
        QTreeWidgetItem *child;
        for(int i = 0; i < item->childCount(); i++)
        {
            child = item->child(i);
            child->setCheckState(0, item->checkState(0));
        }
    }
    else
    {
        if(item->checkState(0) == Qt::Checked)
        {
            filterString_.append('|' + item->text(0));
            emit checkStateChanged(static_cast<GraphicsRegion::Id>(item->data(0, Qt::UserRole).toInt()), true);
        }
        else
        {
            filterString_.remove('|' + item->text(0), Qt::CaseSensitive);
            emit checkStateChanged(static_cast<GraphicsRegion::Id>(item->data(0, Qt::UserRole).toInt()), false);
        }

        //emit checkStateChanged(filterString_);
    }
}
