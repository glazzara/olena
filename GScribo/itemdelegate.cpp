#include "itemdelegate.h"

ItemDelegate::ItemDelegate(QWidget *parent):
        QItemDelegate(parent)
{
}

QWidget * ItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem&, const QModelIndex&) const
{
    QLabel *label = new QLabel(parent);
    label->setMinimumSize(100, 100);

    return label;
}

void ItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QLabel *label = static_cast<QLabel *>(editor);
    QPixmap pixmap = index.model()->data(index, Qt::DecorationRole).value<QPixmap>();
    label->setPixmap(pixmap);
}

void ItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    editor->setGeometry(option.rect);
}
