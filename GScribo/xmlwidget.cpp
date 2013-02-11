#include "xmlwidget.h"

XmlWidget::XmlWidget(QWidget *parent) :
        QWidget(parent)
{
    viewer.setHeaderHidden(true);

    QHBoxLayout *hLayout = new QHBoxLayout;

    QStringList headerNames;
    headerNames << tr("Name") << tr("Value");

    property.setHeaderItem(new QTreeWidgetItem(headerNames));

    hLayout->addWidget(&viewer);
    hLayout->addWidget(&property);

    setLayout(hLayout);

    connect(&viewer, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(setSelectItem(QTreeWidgetItem*)));
}

void XmlWidget::setSelectItem(QTreeWidgetItem *treeItem)
{
    viewer.setCurrentItem(treeItem, 0);
    treeItem->setExpanded(true);

    // Fill property tree view.
    fillProperty(treeItem);

    // Emit signal to select corresponding graphical item on the scene.
    emit selectGraphicalItem(VariantPointer<PolygonItem>::fromQVariant(treeItem->data(0, Qt::UserRole+1)));
}

void XmlWidget::fillProperty(QTreeWidgetItem *treeItem)
{
    property.clear();

    // Get node from the tree item.
    QDomNamedNodeMap *node = VariantPointer<QDomNamedNodeMap>::fromQVariant(treeItem->data(0, Qt::UserRole));

    if(node)
    {
        QStringList values;

        // Fill tree widget with all property of a node.
        for(int i = 0; i < node->count(); i++)
        {
            values << node->item(i).toAttr().name() << node->item(i).toAttr().value();
            property.addTopLevelItem(new QTreeWidgetItem(values));
            values.clear();
        }
    }

    property.resizeColumnToContents(0);
}

void XmlWidget::changeView(QTreeWidgetItem *treeItem)
{
    if(treeItem != viewer.topLevelItem(0))
    {
        // Delete all dom nodes pointers inside items data.
        QTreeWidgetItem *topLevelItem = viewer.takeTopLevelItem(0);
        if(topLevelItem)
        {
            deleteItemsNode(topLevelItem);
            delete VariantPointer<QDomNamedNodeMap>::fromQVariant(topLevelItem->data(0, Qt::UserRole));
            delete topLevelItem;
        }

        // Reset tree widgets.
        property.clear();
        viewer.reset();

        if(treeItem != 0)
            viewer.addTopLevelItem(treeItem);
    }
}

void XmlWidget::deleteItemsNode(QTreeWidgetItem *topLevelItem)
{
    QTreeWidgetItem *item;

    // Delete recursively all childs of an item and the child of the child item etc ...
    for(int i = 0; i < topLevelItem->childCount(); i++)
    {
        item = topLevelItem->child(i);
        deleteItemsNode(item);
        delete VariantPointer<QDomNamedNodeMap>::fromQVariant(item->data(0, Qt::UserRole));
        delete item;
    }
}
