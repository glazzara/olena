#include "xml.h"

Xml::Xml()
{
}

Xml::Xml(const QString &filename)
{
    load(filename);
}

void Xml::load(const QString &filename)
{
    QFile xmlFile(filename);
    xmlFile.open(QIODevice::ReadOnly);

    QDomDocument xml;
    xml.setContent(&xmlFile);

    xmlFile.close();

    QDomElement root = xml.documentElement();
    tItems.setText(0, root.tagName());

    root = root.firstChild().toElement();
    init(root, &tItems);

    root.nextSibling().toElement();
}

void Xml::init(const QDomElement& root, QTreeWidgetItem *parentTreeItem)
{
    QDomElement node = root;
    QTreeWidgetItem *precTreeItem = 0;

    while(!node.isNull())
    {
        QTreeWidgetItem *treeItem = new QTreeWidgetItem(parentTreeItem, precTreeItem);
        treeItem->setText(0, node.tagName());

        // Add widget item child with text content.
        QString nodeText = node.text();
        QTreeWidgetItem *child = new QTreeWidgetItem(treeItem);
        child->setText(0, nodeText);

        precTreeItem = treeItem;
        node = node.nextSibling().toElement();
    }
}

void Xml::processNode(const QDomElement& root, const GraphicRegion::Data& data, QTreeWidgetItem *parentTreeItem)
{

    /*if(!root.isNull())
    {
        QDomElement node = root;

        QTreeWidgetItem *treeItem = new QTreeWidgetItem(parentTreeItem);
        treeItem->setText(0, node.tagName());

        node = node.firstChild().toElement();
        treeItem = new QTreeWidgetItem(treeItem);
        treeItem->setText(0, node.tagName());

        node = node.firstChild().toElement();
        QTreeWidgetItem *precItem = 0;

        if(data.region == GraphicRegion::Text)
        {
            while(!node.isNull())
            {

            }
        }

        else
        {
            while(!node.isNull())
            {

            }
        }

        }*/
}
