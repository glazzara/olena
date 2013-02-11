#ifndef XMLWIDGET_H
#define XMLWIDGET_H

#include <QSortFilterProxyModel>
#include <QGraphicsView>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QDomElement>
#include <QWidget>
#include <QLabel>

#include "selectionproxy.h"
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
        QSortFilterProxyModel proxy_;
        SelectionProxy selectionProxy_;
        XmlModel model_;
        XmlView viewer_;
        XmlView selection_;
        QTreeWidget property_;

    public slots:
        inline void onBeginGraphicalSelection();
        inline void onEndGraphicalSelection();
};

inline void XmlWidget::onBeginGraphicalSelection()
{ selectionProxy_.beginResetModel(); }

inline void XmlWidget::onEndGraphicalSelection()
{ selectionProxy_.endResetModel(); }

#endif // XMLWIDGET_H
