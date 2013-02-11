#include "pageswidget.h"

PagesWidget::PagesWidget(QWidget *parent):
        QListView(parent)
{
    setUniformItemSizes(true);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setViewMode(QListView::IconMode);
    setIconSize(QSize(200, 200));
    setMovement(QListView::Static);
    setResizeMode(QListView::Adjust);
    setSpacing(10);

    setModel(&model_);
    setItemDelegate(new PagesDelegate());

    connect(this, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(getPicture(QModelIndex)));
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
            model_.removePixmap(indexes[i]);

        // Draw new pixmap on the scene.
        getPicture(model_.index(indexes[0].row()));
        clearSelection();
    }
}
