#ifndef XML_H
#define XML_H

#include <QTreeWidgetItem>
#include <QGraphicsItem>
#include <QStringList>
#include <QDomElement>
#include <climits>
#include <QFile>

#include "Rendering/polygonitem.h"
#include "variantpointer.h"
#include "configs.h"
#include "region.h"

class Xml
{
    public:
        explicit Xml(const QString& filename = QString());

        inline QTreeWidgetItem *treeItem();
        inline QGraphicsItem *graphicItem();
        inline QDomDocument document() const;
        inline QString filename() const;

        static QString getPath(const QString& filename);
        void load(const QString& filename);

    private:
        QTreeWidgetItem *init(const QDomElement& root, QTreeWidgetItem *rootTreeItem);
        inline QTreeWidgetItem *fillWidgetItem(const QString& tagName, QTreeWidgetItem *rootTreeItem);
        void processNode(const QDomElement& root, const GraphicRegion::Data& data, QTreeWidgetItem *rootTreeItem);
        void processLineNode(const QDomElement& root, PolygonItem *parentPolygonItem, QTreeWidgetItem *rootTreeItem);
        void processTypoNode(const QDomElement& root, const QPoint& xPoint, PolygonItem *rootPolygonItem);

        QGraphicsPolygonItem *gItem;
        QTreeWidgetItem *tItem;
        QDomDocument xml;
        QString path;
};

inline QTreeWidgetItem *Xml::treeItem()
{ return tItem; }

inline QGraphicsItem *Xml::graphicItem()
{ return gItem; }

inline QDomDocument Xml::document() const
{ return xml; }

inline QString Xml::filename() const
{ return path; }

inline QTreeWidgetItem *Xml::fillWidgetItem(const QString& tagName, QTreeWidgetItem *rootTreeItem)
{
    QTreeWidgetItem *treeItem = new QTreeWidgetItem(rootTreeItem, QStringList(tagName));
    treeItem = new QTreeWidgetItem(treeItem, QStringList("Coords"));
    return treeItem;
}

#endif // XML_H
