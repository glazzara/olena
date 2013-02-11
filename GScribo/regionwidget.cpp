#include "regionwidget.h"

RegionWidget::RegionWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    setHeaderHidden(true);

    addTopLevelItem(createRoot("Text", GraphicsRegion::TextRegion, GraphicsRegion::Text, GraphicsRegion::Line));
    addTopLevelItem(createRoot("Typological Lines", GraphicsRegion::TypoRegion, GraphicsRegion::Baseline, GraphicsRegion::Meanline));
    addTopLevelItem(createRoot("Separators", GraphicsRegion::SeparatorRegion, GraphicsRegion::VerticalSeparator, GraphicsRegion::WhiteSpaceSeparator));
    addTopLevelItem(createRoot("Miscellaneous", GraphicsRegion::MiscRegion, GraphicsRegion::Image, GraphicsRegion::Chart));

    expandAll();

    connect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(onItemChanged(QTreeWidgetItem*)));
}

RegionWidgetItem *RegionWidget::createRoot(const QString& text, const GraphicsRegion::Id& region, const GraphicsRegion::Id& begin, const GraphicsRegion::Id& end)
{
    // Create root.
    RegionWidgetItem *rootItem = createItem(text, region);

    // Fill root with corresponding childs.
    for(int region_ = begin; region_ <= end; region_++)
        fillRoot(rootItem, static_cast<GraphicsRegion::Id>(region_));

    return rootItem;
}

RegionWidgetItem *RegionWidget::createItem(const QString& text, const GraphicsRegion::Id& region, const QColor& color)
{
    RegionWidgetItem *item = new RegionWidgetItem(region);
    item->setText(0, text);
    item->setBackgroundColor(0, color);

    return item;
}

void RegionWidget::onItemChanged(QTreeWidgetItem *item)
{
    RegionWidgetItem *regionWidgetItem = static_cast<RegionWidgetItem *>(item);

    if(regionWidgetItem->isCheckStateChanged())
        emit checkStateChanged(regionWidgetItem->region());
}
