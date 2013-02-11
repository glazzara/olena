#ifndef XML_H
#define XML_H

#include <QTreeWidgetItem>
#include <QGraphicsItem>
#include <QStringList>
#include <QDomElement>
#include <climits>
#include <QFile>

#include "Rendering/rootgraphicsitem.h"
#include "Rendering/polygonitem.h"
#include "XmlWidget/xmlitem.h"
#include "variantpointer.h"
#include "configs.h"
#include "region.h"

class Xml
{
    public:
        explicit Xml(const QString& filename = QString());

        inline XmlItem *xmlItem();
        inline RootGraphicsItem *graphicItem();
        inline QDomDocument document() const;
        inline QString filename() const;

        static QString getPath(const QString& filename);
        void load(const QString& filename);

    private:
        XmlItem *init(const QDomElement& root, XmlItem *rootTreeItem);
        void processNode(const QDomElement& root, const GraphicRegion::Data& data, XmlItem *rootTreeItem);
        void processLineNode(const QDomElement& root, XmlItem *rootTreeItem);
        void processTypoNode(const QDomElement& root, const QPoint& xPoint, PolygonItem *rootPolygonItem);

        RootGraphicsItem *graphicalItem_;
        XmlItem *xmlItem_;
        QDomDocument xml_;
        QString filename_;
};

inline XmlItem *Xml::xmlItem()
{ return xmlItem_; }

inline RootGraphicsItem *Xml::graphicItem()
{ return graphicalItem_; }

inline QDomDocument Xml::document() const
{ return xml_; }

inline QString Xml::filename() const
{ return filename_; }

#endif // XML_H
