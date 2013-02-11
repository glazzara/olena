#ifndef PAGESWIDGET_H
#define PAGESWIDGET_H

#include <QListView>
#include <QShortcut>

#include "listmodel.h"

class PagesWidget:
        public QListView
{
        Q_OBJECT

    public:
        explicit PagesWidget(QWidget *parent = 0);
        void addPixmap(const QString& filename, const QPixmap& pixmap);
        QStringList filenames() const;

    private:
        ListModel model;

    public slots:
        void getPixmap(const QModelIndex &index);
        void removeSelection();

    signals:
        void clicked(const QModelIndex &index);
        void selectionClicked(const QString& filename, const QPixmap& pixmap);
};

#endif // PAGESWIDGET_H
