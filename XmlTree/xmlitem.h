#ifndef XMLITEM_H
#define XMLITEM_H

#include <QDomElement>

#include "polygonitem.h"

class XmlItem
{
    public:
        explicit XmlItem(XmlItem *parent = 0, XmlItem *precItem = 0);
        explicit XmlItem(const QDomElement& node, XmlItem *parent = 0, XmlItem *precItem = 0);
        ~XmlItem();

        void load(const QDomElement& node);

        inline QList<XmlItem *> childs() const;
        inline void addChild(XmlItem *child);
        inline XmlItem *child(int i);
        inline XmlItem *parent();

        inline PolygonItem *graphicalItem() const;
        inline void setGraphicalItem(PolygonItem *graphicalItem);

        inline QString text() const;
        inline QList<QStringList> attributes() const;
        inline int rowCount() const;

    private:
        void init(XmlItem *parent = 0, XmlItem *precItem = 0);

        QList<XmlItem *> listChilds;
        XmlItem *father;
        PolygonItem *gItem;

        QString tagName;
        QList<QStringList> attrs;

        int row;
};

inline QList<XmlItem *> XmlItem::childs() const
{ return listChilds; }

inline void XmlItem::addChild(XmlItem *child)
{ listChilds << child; }

inline XmlItem *XmlItem::child(int i)
{ if(i > -1 && i < listChilds.count()) return listChilds[i]; return 0; }

inline XmlItem *XmlItem::parent()
{ return father; }

inline PolygonItem *XmlItem::graphicalItem() const
{ return gItem; }

inline void XmlItem::setGraphicalItem(PolygonItem *graphicalItem)
{ gItem = graphicalItem; }

inline QString XmlItem::text() const
{ return tagName; }

inline QList<QStringList> XmlItem::attributes() const
{ return attrs; }

inline int XmlItem::rowCount() const
{ return row; }

Q_DECLARE_METATYPE(XmlItem);

#endif // XMLITEM_H
