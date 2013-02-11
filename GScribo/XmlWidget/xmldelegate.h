#ifndef XMLDELEGATE_H
#define XMLDELEGATE_H

#include <QStyledItemDelegate>
#include<QPainter>

class XmlDelegate :
        public QStyledItemDelegate
{
    public:
        explicit XmlDelegate(int role = Qt::DisplayRole, QObject *parent = 0) : QStyledItemDelegate(parent) { role_ = role; }
        inline void paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;

        inline void setRole(int role);
        inline int role() const;

    private:
        int role_;
};

inline void XmlDelegate::paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{ QStyledItemDelegate::paint(painter, option, index); painter->drawText(option.rect, index.data(role_).toString()); }

inline void XmlDelegate::setRole(int role)
{ role_ = role; }

inline int XmlDelegate::role() const
{ return role_; }

#endif // XMLDELEGATE_H
