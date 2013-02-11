#include "xmlview.h"

XmlView::XmlView(QWidget *parent) :
        QTreeView(parent)
{
    proxy_.setFilterRole(Qt::UserRole+1);
    proxy_.setDynamicSortFilter(true);

    setUniformRowHeights(true);
    setItemDelegate(new XmlDelegate(Qt::UserRole));
    setSelectionMode(QAbstractItemView::ExtendedSelection);
}

void XmlView::hideVisibles()
{
    XmlItem *child;
    foreach(child, visibleItems_)
        child->hide();

    visibleItems_.clear();
}

void XmlView::onEndGraphicalSelection(const QList<QGraphicsItem *>& selectedItems)
{
    hideVisibles();

    if(selectedItems.isEmpty())
        proxy_.selectAll();
    else
    {
        QGraphicsItem *child;
        XmlItem *xmlItem;
        foreach(child, selectedItems)
        {
            xmlItem = static_cast<PolygonItem *>(child)->xmlItem();
            xmlItem->show();
            visibleItems_ << xmlItem;
        }
    }

    proxy_.endResetModel();

    emit resetAttributes();
    expandAll();
}

void XmlView::selectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
    QTreeView::selectionChanged(selected, deselected);

    if(!proxy_.isAllSelected())
    {
        //if(emptySelection_)
            //emit resetGraphicalSelection();

        if(!selected.isEmpty())
        {
            QList<PolygonItem *> sel;
            QModelIndex index;

            foreach(index, selected.indexes())
            {
                PolygonItem *graphicalItem = VariantPointer<PolygonItem>::fromQVariant(index.data(Qt::UserRole+3));

                if(graphicalItem)
                    sel << graphicalItem;
            }

            //emptySelection_ = false;
            emit selection(sel, true);
        }
        //else
            //emit resetGraphicalSelection();
    }
    //else
        //emptySelection_ = true;
}
