#ifndef PAGESWIDGET_H
#define PAGESWIDGET_H

#include <QPushButton>
#include <QScrollBar>
#include <QListView>
#include <QPainter>
#include <QWidget>

#include "listmodel.h"

class PagesWidget:
        public QListView
{
        Q_OBJECT

    public:
        explicit PagesWidget(QWidget *parent = 0);
        void addPixmap(const QString& filename, const QPixmap& pixmap);

    private:
        ListModel model;

    public slots:
        void getSelection(const QModelIndex &index);

    signals:
        void clicked(const QModelIndex &index);
        void selectionClicked(const QString& filename, const QPixmap& pixmap);
};

#endif // PAGESWIDGET_H
