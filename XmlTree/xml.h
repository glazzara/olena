#ifndef XML_H
#define XML_H

#include <QTreeWidgetItem>
#include <QGraphicsItem>
#include <QStringList>
#include <QDomElement>
#include <QFile>

#include "polygonitem.h"
#include "region.h"

class Xml
{
    public:
        Xml();
        explicit Xml(const QString& filename);
        void load(const QString& filename);
        inline QTreeWidgetItem *treeItem();

    private:
        void init(const QDomElement& root, QTreeWidgetItem *parent);
        void processNode(const QDomElement& root, const GraphicRegion::Data& data, QTreeWidgetItem *parent);

        QGraphicsItem *graphicsText(const QDomElement& root);
        /*void graphicsLine(const QDomeElement& root, QGraphicsItem *graphicsText);
        void graphicsTypo(const QDomeElement& root, QGraphicsItem *graphicsLine);*/

        void fillTreeWidgetItem(const QDomElement& root, QTreeWidgetItem *parent);
        QGraphicsItem *graphicsRegion(const QDomElement& root);

        QList<QGraphicsItem *> gItems;
        QTreeWidgetItem tItems;
};

inline QTreeWidgetItem *Xml::treeItem()
{ return &tItems; }

#endif // XML_H
