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

    QShortcut *remove = new QShortcut(Qt::Key_Delete, this);
    connect(remove, SIGNAL(activated()), this, SLOT(removeSelection()));
    connect(this, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(getPicture(QModelIndex)));
}

void PagesWidget::removeSelection()
{
    if(hasFocus() && selectedIndexes().count() > 0)
    {
        // Sort the list because after the first removal, the second row index may not be the good one.
        qSort(selectedIndexes().begin(), selectedIndexes().end());

        // From the highest row to the smallest to be sure to remove the good index.
        for(int i = selectedIndexes().count()-1; i > -1; i--)
            model_.removePixmap(selectedIndexes()[i]);

        // Draw new image on the scene.
        getPicture(model_.index(selectedIndexes()[0].row()));
        clearSelection();

        model_.setCurrentRow(selectedIndexes()[0].row());
    }
}

void PagesWidget::getPicture(const QModelIndex& index)
{
    QString filename = index.data(Qt::UserRole).toString();
    model_.setCurrentRow(index.row());

    emit imageSelectionned(filename);
}
