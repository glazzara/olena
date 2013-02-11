#include "xmlitem.h"

XmlItem::XmlItem(XmlItem *parent, XmlItem *precItem)
{
    init(parent, precItem);
}

XmlItem::XmlItem(const QDomElement &node, XmlItem *parent, XmlItem *precItem)
{
    init(parent, precItem);
    load(node);
}

XmlItem::~XmlItem()
{
    // Delete all childs.
    XmlItem *child;
    foreach(child, childs())
        delete child;
}

void XmlItem::init(XmlItem *parent, XmlItem *precItem)
{
    if(parent)
        parent->addChild(this);

    gItem = 0;
    father = parent;

    if(precItem)
        row = precItem->rowCount()+1;
    else
        row = 0;
}

void XmlItem::load(const QDomElement& node)
{
    tagName = node.tagName();

    // Load node attributes.
    QDomNamedNodeMap nodeMap = node.attributes();
    QDomAttr attr;
    QStringList values;

    for(int i = 0; i < nodeMap.count(); i++)
    {
        attr = nodeMap.item(i).toAttr();
        values << attr.name() << attr.value();

        attrs << values;
    }
}
