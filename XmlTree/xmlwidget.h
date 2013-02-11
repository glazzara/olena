#ifndef XMLWIDGET_H
#define XMLWIDGET_H

#include <QTreeWidgetItem>
#include <QDomElement>
#include <QWidget>

class XmlWidget :
        public QWidget
{
    friend class QTreeWidget;

    public:
        explicit XmlWidget(QTreeWidgetItem *item = 0);

        inline void load(QTreeWidgetItem *item);

    private:
        QTreeWidget tree;
};

inline void XmlWidget::load(QTreeWidgetItem *item)
{ tree.addTopLevelItem(item); }

#endif // XMLWIDGET_H
