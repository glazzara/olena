#include "pageswidget.h"

PagesWidget::PagesWidget(QWidget *parent):
        QListView(parent)
{
    setUniformItemSizes(true);
    setViewMode(QListView::IconMode);
    setIconSize(QSize(200, 200));
    setFlow(QListView::TopToBottom);
    setMovement(QListView::Static);
    setSpacing(10);
    setWrapping(false);
    setModel(&model);

    connect(this, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(getSelection(QModelIndex)));
}

void PagesWidget::addPixmap(const QString& filename, const QPixmap &pixmap)
{
    model.addPixmap(filename, pixmap);
}

void PagesWidget::getSelection(const QModelIndex &index)
{
    // Get the pixmap and the path of the icon to send it to the scene and draw it.
    QPixmap pixmap = index.data(Qt::UserRole).value<QPixmap>();
    QString filename = index.data(Qt::UserRole+1).toString();

    emit selectionClicked(filename, pixmap);
}
