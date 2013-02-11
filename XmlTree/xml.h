#ifndef XML_H
#define XML_H

#include <QGraphicsItem>
#include <QStringList>
#include <QDomElement>
#include <climits>
#include <QFile>

#include "polygonitem.h"
#include "variantpointer.h"
#include "xmlitem.h"
#include "configs.h"
#include "region.h"

class Xml
{
    public:
        explicit Xml(const QString& filename = QString());

        inline XmlItem *treeItem();
        inline QGraphicsItem *graphicItem();
        inline QDomDocument document() const;
        inline QString filename() const;

        static QString getPath(const QString& filename);
        void load(const QString& filename);

    private:
        XmlItem *init(const QDomElement& root, XmlItem *rootTreeItem);
        //inline QTreeWidgetItem *fillWidgetItem(const QString& tagName, QTreeWidgetItem *rootTreeItem);
        void processNode(const QDomElement& root, const GraphicRegion::Data& data, XmlItem *rootTreeItem);
        void processLineNode(const QDomElement& root, PolygonItem *parentPolygonItem, XmlItem *rootTreeItem);
        void processTypoNode(const QDomElement& root, const QPoint& xPoint, PolygonItem *rootPolygonItem);

        QGraphicsPolygonItem *gItem;
        XmlItem *tItem;
        QDomDocument xml;
        QString path;
};

inline XmlItem *Xml::treeItem()
{ return tItem; }

inline QGraphicsItem *Xml::graphicItem()
{ return gItem; }

inline QDomDocument Xml::document() const
{ return xml; }

inline QString Xml::filename() const
{ return path; }

/*inline XmlItem *Xml::fillWidgetItem(const QString& tagName, QTreeWidgetItem *rootTreeItem)
{
    QTreeWidgetItem *treeItem = new QTreeWidgetItem(rootTreeItem, QStringList(tagName));
    treeItem = new QTreeWidgetItem(treeItem, QStringList("Coords"));
    return treeItem;
}*/

#endif // XML_H
