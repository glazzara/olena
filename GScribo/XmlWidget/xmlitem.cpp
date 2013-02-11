#include "xmlitem.h"

XmlItem::XmlItem(XmlItem *parent, XmlItem *precItem)
{
    init(parent, precItem);
}

XmlItem::XmlItem(const QDomElement& node, XmlItem *parent, XmlItem *precItem)
{
    init(parent, precItem);
    load(node);
}

XmlItem::~XmlItem()
{
    // Delete all childs.
    XmlItem *child;
    foreach(child, childs_)
        delete child;
}

void XmlItem::init(XmlItem *parent, XmlItem *precItem)
{
    isSelected_ = true;
    graphicalItem_ = 0;
    parent_ = parent;

    if(parent)
        parent->addChild(this);

    if(precItem)
        row_ = precItem->row()+1;
    else
        row_ = 0;
}

void XmlItem::load(const QDomElement& node)
{
    text_ = node.tagName();

    // Load attributes.
    attributes_.load(node.attributes());
}
