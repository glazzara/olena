#ifndef XMLDELEGATE_H
#define XMLDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QSpinBox>
#include <QLabel>

class XmlDelegate :
        public QStyledItemDelegate
{
    public:
        explicit XmlDelegate(QObject *parent = 0);

        //QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
        /*void setEditorData(QWidget *editor, const QModelIndex& index) const;
        void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem& option, const QModelIndex& index) const;*/
};

#endif // XMLDELEGATE_H
