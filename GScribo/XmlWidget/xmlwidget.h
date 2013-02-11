#ifndef XMLWIDGET_H
#define XMLWIDGET_H

#include <QGraphicsView>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QDomElement>
#include <QWidget>
#include <QLabel>

#include "Rendering/polygonitem.h"
#include "treedelegate.h"
#include "xmlmodel.h"
#include "xmlview.h"

class XmlWidget :
        public QWidget
{
        Q_OBJECT

    public:
        explicit XmlWidget(QWidget *parent = 0);

        void changeView(XmlItem *rootItem);

    private:

        XmlView viewer_;
        QTreeWidget selection_;
        QTreeWidget property_;
};

#endif // XMLWIDGET_H
