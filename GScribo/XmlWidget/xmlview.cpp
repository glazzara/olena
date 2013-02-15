#include "xmlview.h"

XmlView::XmlView(QWidget *parent) :
        QTreeView(parent)
{
    proxy_.setFilterRole(Qt::UserRole);
    proxy_.setDynamicSortFilter(true);

    setUniformRowHeights(true);
    setItemDelegate(new XmlDelegate(Qt::UserRole));
    setSelectionMode(QAbstractItemView::ExtendedSelection);
}

void XmlView::hideAll()
{
    proxy_.beginResetModel();

    XmlItem *child;
    foreach(child, displayedItems_)
        child->hide();

    displayedItems_.clear();
    emit resetProperty();

    proxy_.endResetModel();
}

void XmlView::display(QList<XmlItem *> displayedItems, bool addToDisplayedList)
{
    proxy_.beginResetModel();

    if(!addToDisplayedList)
        hideAll();

    XmlItem *child;
    foreach(child, displayedItems)
    {
        child->show();
        displayedItems_ << child;
    }

    proxy_.endResetModel();

    expandAll();
}

QList<XmlItem *> XmlView::transform(const QModelIndexList& modelIndexList) const
{
    QList<XmlItem *> selectedItems;
    QModelIndex index;

    foreach(index, modelIndexList)
        selectedItems << index.data(Qt::UserRole+2).value<XmlItem *>();

    return selectedItems;
}

void XmlView::selectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
    QTreeView::selectionChanged(selected, deselected);

    if(selected.indexes() == selectedIndexes())
        emit resetSelection();

    if(!selectedIndexes().isEmpty())
    {
        QList<XmlItem *> xmlItems;
        if(!selected.isEmpty())
        {
            xmlItems = transform(selected.indexes());
            emit select(xmlItems);
        }
        else
        {
            xmlItems = transform(deselected.indexes());
            emit unselect(xmlItems);
        }

        emit loadAttributes(xmlItems.last()->attributes());
    }
    else
    {
        emit resetProperty();
        emit emptySelection();
    }

    emit newSelection();
}
