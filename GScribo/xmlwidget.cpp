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

    connect(&viewer, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(fillProperty(QTreeWidgetItem*)));
}

void XmlWidget::fillProperty(QTreeWidgetItem *treeItem)
{
    property.clear();

    // Get node from the tree item.
    QDomElement *node = VariantPointer<QDomElement>::fromQVariant(treeItem->data(0, Qt::UserRole));
    QStringList values;

    if(node->hasAttributes())
    {
        QDomNamedNodeMap attributes = node->attributes();

        for(int i = 0; i < attributes.count(); i++)
        {
            values << attributes.item(i).toAttr().name() << attributes.item(i).toAttr().value();
            property.addTopLevelItem(new QTreeWidgetItem(values));
            values.clear();
        }
    }
    property.resizeColumnToContents(0);
}
