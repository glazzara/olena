#ifndef XMLITEM_H
#define XMLITEM_H

#include <QDomElement>

#include "xmlattributes.h"

class PolygonItem;

class XmlItem
{
    public:
        explicit XmlItem(XmlItem *parent = 0, XmlItem *precItem = 0);
        explicit XmlItem(const QDomElement& node, XmlItem *parent = 0, XmlItem *precItem = 0);
        ~XmlItem();

        void load(const QDomElement& node);

        inline QList<XmlItem *> childs() const;
        inline XmlItem *child(int i) const;
        inline void addChild(XmlItem *child);
        inline XmlItem *parent() const;

        inline PolygonItem *graphicalItem();
        inline void setGraphicalItem(PolygonItem *graphicalItem);

        inline bool isSelected() const;
        inline void setSelected(bool selected);
        inline void select();
        inline void unselect();

        inline QString text() const;
        inline int row() const;
        inline XmlAttributes attributes() const;

    private:
        void init(XmlItem *parent = 0, XmlItem *precItem = 0);

        int row_;
        bool isSelected_;
        QList<XmlItem *> childs_;
        XmlItem *parent_;
        PolygonItem *graphicalItem_;
        QString text_;
        XmlAttributes attributes_;
};

inline QList<XmlItem *> XmlItem::childs() const
{ return childs_; }

inline XmlItem *XmlItem::child(int i) const
{ if(i > -1 && i < childs_.count()) return childs_[i]; return 0; }

inline void XmlItem::addChild(XmlItem *child)
{ childs_ << child; }

inline XmlItem *XmlItem::parent() const
{ return parent_; }

inline PolygonItem *XmlItem::graphicalItem()
{ return graphicalItem_; }

inline void XmlItem::setGraphicalItem(PolygonItem *graphicalItem)
{ graphicalItem_ = graphicalItem; }

inline bool XmlItem::isSelected() const
{ return isSelected_; }

inline void XmlItem::setSelected(bool selected)
{ isSelected_ = selected; }

inline void XmlItem::select()
{ setSelected(true); }

inline void XmlItem::unselect()
{ setSelected(false); }

inline QString XmlItem::text() const
{ return text_; }

inline int XmlItem::row() const
{ return row_; }

inline XmlAttributes XmlItem::attributes() const
{ return attributes_; }

#endif // XMLITEM_H
