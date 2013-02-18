#ifndef XMLWIDGET_H
#define XMLWIDGET_H

#include <QTreeWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>

class XmlWidget :
        public QWidget
{
    public:
        explicit XmlWidget(QWidget *parent = 0);

        inline void load(QTreeWidgetItem *treeItem);

    private:
        QTreeWidget viewer;
        QTreeWidget property;
};

inline void XmlWidget::load(QTreeWidgetItem *treeItem)
{ if(treeItem != 0) { viewer.reset(); viewer.addTopLevelItem(treeItem); } }

#endif // XMLWIDGET_H
