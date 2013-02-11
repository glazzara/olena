#ifndef TREEVIEW_H
#define TREEVIEW_H

#include <QTreeView>

#include "xmlmodel.h"

class TreeView : public QTreeView
{
    public:
        explicit TreeView(QWidget *parent);
        explicit TreeView(QTreeViewPrivate& dd, QWidget *parent);
        explicit TreeView(const QTreeView& view);

    private:
        XmlModel model;
};

#endif // TREEVIEW_H
