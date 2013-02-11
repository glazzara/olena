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
}

void PagesWidget::addPixmap(const QString& filename, const QPixmap &pixmap)
{
    model.addPixmap(pixmap);
}
