#include "xmlwidget.h"

XmlWidget::XmlWidget(QWidget *parent) :
        QWidget(parent)
{
    QHBoxLayout *hLayout = new QHBoxLayout;

    QStringList headerNames;
    headerNames << tr("Name") << tr("Value");

    property.setHeaderItem(new QTreeWidgetItem(headerNames));
    viewer.setHeaderItem(new QTreeWidgetItem(QStringList("Tree")));
    selection.setHeaderItem(new QTreeWidgetItem(QStringList("Selection")));

    hLayout->addWidget(&viewer);
    hLayout->addWidget(&selection);
    hLayout->addWidget(&property);

    setLayout(hLayout);

    connect(&viewer, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(selectTreeItem(QTreeWidgetItem*)));
    connect(&selection, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(selectSelectedItem(QTreeWidgetItem*)));
}

XmlWidget::~XmlWidget()
{
    clear();
}

void XmlWidget::selectItem(QTreeWidgetItem *treeItem, bool fromSelection)
{
    // Get graphical item corresponding to the tree item.
    PolygonItem *graphicalItem = VariantPointer<PolygonItem>::fromQVariant(treeItem->data(0, Qt::UserRole+1));

    if(!fromSelection)
    {
        clearSelection();

        QTreeWidgetItem *selectionedItem = new QTreeWidgetItem(*treeItem);
        selectionedItems << selectionedItem;
        selection.addTopLevelItem(selectionedItem);
    }

    // Told to the scene to select the corresponding graphical item.
    emit selectGraphicalItem(graphicalItem);

    viewer.setCurrentItem(treeItem, 0);

    // Fill property tree view.
    fillProperty(treeItem);
}

void XmlWidget::selectItems(const QList<QTreeWidgetItem *>& selectionTree)
{
    clearSelection();

    if(!selectionTree.isEmpty())
    {
        selectionedItems = selectionTree;

        QTreeWidgetItem *treeItem;
        foreach(treeItem, selectionTree)
        {
            selection.addTopLevelItem(treeItem);
            /*selection.setCurrentItem(treeItem, 0);
            treeItem->setExpanded(true);*/
        }

        fillProperty(treeItem);
    }
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
        clear();

        // Reset tree widgets.
        viewer.reset();

        if(treeItem != 0)
        {
            viewer.addTopLevelItem(treeItem);
            viewer.setCurrentItem(treeItem->child(1), 0);
            treeItem->child(1)->setExpanded(true);
        }
    }
}

void XmlWidget::clear()
{
    QTreeWidgetItem *topLevelItem = viewer.takeTopLevelItem(0);

    // Delete top level item, all its childs and datas.
    if(topLevelItem)
    {
        deleteItemsNode(topLevelItem);
        delete VariantPointer<QDomNamedNodeMap>::fromQVariant(topLevelItem->data(0, Qt::UserRole));
        delete topLevelItem;

        clearSelection();
    }
}

void XmlWidget::clearSelection()
{
    property.clear();

    // Delete all selectionned items.
    QTreeWidgetItem *item, *child;
    foreach(item, selectionedItems)
    {
        // Delete selectionned lines.
        for(int i = 0; i < item->childCount(); i++)
        {
            child = item->child(i);
            delete child;
        }

        delete item;
    }

    selectionedItems = QList<QTreeWidgetItem *>();
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
