#include "pageswidget.h"

PagesWidget::PagesWidget(QWidget *parent):
        QListView(parent)
{
    setUniformItemSizes(true);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setSelectionRectVisible(false);
    setViewMode(QListView::IconMode);
    setIconSize(QSize(200, 200));
    setFlow(QListView::TopToBottom);
    setMovement(QListView::Static);
    setSpacing(10);
    setWrapping(false);
    setModel(&model);

    connect(this, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(getPixmap(QModelIndex)));
}

QStringList PagesWidget::filenames() const
{
    return model.filenames();
}

void PagesWidget::getPixmap(const QModelIndex& index)
{
    // Get the pixmap and the path of the icon to send it to the scene and draw it.
    QPixmap pixmap = index.data(Qt::UserRole).value<QPixmap>();
    QString filename = index.data(Qt::UserRole+1).toString();

    emit sceneChanged(filename, pixmap);
}

void PagesWidget::addPixmap(const QString& filename, const QPixmap& pixmap)
{
    model.addPixmap(filename, pixmap);
}

void PagesWidget::removeSelection()
{
    QModelIndexList indexes = selectedIndexes();

    if(indexes.count() > 0)
    {
        // We have to sort the list because after the first removal, the second row index may not be the good one.
        qSort(indexes.begin(), indexes.end());

        // From the highest row value to the smallest to be sure to remove the good index.
        for(int i = indexes.count()-1; i > -1; i--)
            model.removePixmap(indexes[i]);

        // Draw new pixmap on the scene.
        getPixmap(model.index(indexes[0].row()));
        clearSelection();
    }
}
