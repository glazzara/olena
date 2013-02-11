#ifndef PAGESWIDGET_H
#define PAGESWIDGET_H

#include <QWidget>
#include <QListView>
#include <QPainter>
#include <QPushButton>
#include <QScrollBar>

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
};

#endif // PAGESWIDGET_H
