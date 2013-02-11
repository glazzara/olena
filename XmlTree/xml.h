#ifndef XML_H
#define XML_H

#include <QTreeWidgetItem>
#include <QGraphicsItem>
#include <QDomElement>
#include <QFile>

class Xml
{
    public:
        Xml();
        explicit Xml(const QString& filename);
        void load(const QString& filename);
        QList<QGraphicsItem *> graphicsItems() const;
        QTreeWidgetItem * treeItem() const;

    private:
        QList<QGraphicsItem *> gItems;
        QTreeWidgetItem tItems;
};

#endif // XML_H
