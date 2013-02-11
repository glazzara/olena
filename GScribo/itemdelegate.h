#ifndef ITEMDELEGATE_H
#define ITEMDELEGATE_H

#include <QItemDelegate>
#include <QLabel>

class ItemDelegate:
        public QItemDelegate
{
    public:
        explicit ItemDelegate(QWidget *parent = 0);
        QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
        void setEditorData(QWidget *editor, const QModelIndex &index) const;
        void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // ITEMDELEGATE_H
