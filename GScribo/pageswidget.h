#ifndef PAGESWIDGET_H
#define PAGESWIDGET_H

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QListWidget>
#include <QStandardItemModel>
#include <QLabel>

#include "itemdelegate.h"
#include "listmodel.h"

class PagesWidget:
        public QWidget
{
        Q_OBJECT

    public:
        explicit PagesWidget(QWidget *parent = 0);
        ~PagesWidget();
        void addPicture(const QString& filename, const QPixmap& pixmap);

    private:
        QListView *listView;
        ListModel *model;
        QStringList *filenames;
};

#endif // PAGESWIDGET_H
