#ifndef XMLITEM_H
#define XMLITEM_H

#include <QDomElement>

#include "Rendering/polygonitem.h"
#include "xmlattributes.h"

class XmlItem
{
    public:
        explicit XmlItem(XmlItem *parent = 0);
        explicit XmlItem(XmlItem *precItem, XmlItem *parent = 0);
        explicit XmlItem(const QDomElement& node, XmlItem *parent = 0);
        explicit XmlItem(const QDomElement& node, XmlItem precItem, XmlItem *parent = 0);
        ~XmlItem();

        void load(const QDomElement& node);

        inline QList<XmlItem *> childs() const;
        inline XmlItem *child(int i) const;
        inline void setChild(XmlItem *child);

        inline XmlItem *parent() const;
        inline void setParent(XmlItem *parent);

        inline PolygonItem *graphicalItem();
        inline void setGraphicalItem(PolygonItem *graphicalItem);

        inline QString text() const;
        inline int row() const;
        inline XmlAttributes *attributes() const;

    private:
        void init(XmlItem *parent = 0, XmlItem *precItem = 0);

        QList<XmlItem *> lChilds;
        XmlItem *par;
        PolygonItem *graphItem;
        QString name;
        XmlAttributes *attrs;
};

#endif // XMLITEM_H
