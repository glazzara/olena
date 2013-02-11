#include "regionwidgetitem.h"

RegionWidgetItem::RegionWidgetItem(const GraphicsRegion::Id& region)
{
    region_ = region;
    isCheckStateChanged_ = false;
    QTreeWidgetItem::setCheckState(0, Qt::Checked);
}

void RegionWidgetItem::setCheckState(int column, Qt::CheckState state)
{
    // Look if the check state has changed or not.
    if(checkState(column) != state)
        isCheckStateChanged_ = true;
    else
        isCheckStateChanged_ = false;

    QTreeWidgetItem::setCheckState(column, state);
}
