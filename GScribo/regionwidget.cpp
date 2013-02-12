#include "regionwidget.h"
#include <QDebug>

RegionWidget::RegionWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    setHeaderHidden(true);
    setSelectionMode(QAbstractItemView::NoSelection);

    Configs *const configs = Configs::getInstance();

    configs->beginGroup("region");

    foreach(QString group, configs->childGroups())
    {
        configs->beginGroup(group);
        GraphicsRegion::Id region = static_cast<GraphicsRegion::Id>(group.toInt());
        QTreeWidgetItem *rootItem = createItem(static_cast<GraphicsRegion::Id>(group.toInt()));

        foreach(QString subgroup, configs->childGroups())
            rootItem->addChild(createItem(static_cast<GraphicsRegion::Id>(subgroup.toInt())));

        addTopLevelItem(rootItem);
        configs->endGroup();
    }

    configs->endGroup();

    expandAll();

    connect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(checkStateChanged(QTreeWidgetItem*)));
}

QTreeWidgetItem *RegionWidget::createItem(const GraphicsRegion::Id& region)
{
    Configs *const configs = Configs::getInstance();

    QTreeWidgetItem *item = new QTreeWidgetItem;

    item->setText(0, configs->regionName(region));
    item->setCheckState(0, configs->isRegionChecked(region) ? Qt::Checked : Qt::Unchecked);
    item->setBackgroundColor(0, configs->regionColor(region));
    // Store graphical id in the object to recognize it afterward.
    item->setData(0, Qt::UserRole, static_cast<int>(region));

    return item;
}

void RegionWidget::checkStateChanged(QTreeWidgetItem *item)
{
    GraphicsRegion::Id region = static_cast<GraphicsRegion::Id>(item->data(0, Qt::UserRole).toInt());
    bool isChecked;

    // If it's a root item, go to childs.
    if(item->childCount() != 0)
    {
        isChecked = item->checkState(0) == Qt::Checked ? true : false;

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
            isChecked = true;
        }
        else
        {
            filterString_.remove('|' + item->text(0), Qt::CaseSensitive);
            isChecked = false;
        }

        emit checkStateChanged(region, isChecked);
        emit checkStateChanged(filterString());
    }

    Configs::getInstance()->setRegionChecked(region, isChecked);
}
