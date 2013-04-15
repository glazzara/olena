#include "xmlattributes.h"

XmlAttributes::XmlAttributes()
{
}

XmlAttributes::XmlAttributes(const QDomNamedNodeMap& node)
{
    load(node);
}

void XmlAttributes::load(const QDomNamedNodeMap& node)
{
    QDomAttr attr;

    for(int i = 0; i < node.count(); i++)
    {
        attr = node.item(i).toAttr();

        names_ << attr.name();
        values_ << attr.value();
    }
}
