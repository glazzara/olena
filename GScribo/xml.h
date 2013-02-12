#ifndef XML_H
#define XML_H

#include <QTreeWidgetItem>
#include <QGraphicsItem>
#include <QStringList>
#include <QDomElement>
#include <climits>
#include <QFile>

#include "Rendering/rootgraphicsitem.h"
#include "Rendering/regionitem.h"
#include "XmlWidget/xmlitem.h"
#include "variantpointer.h"
#include "configs.h"
#include "region.h"

class Xml
{
    public:
        explicit Xml(const QString& filename = QString());

        inline XmlItem *xmlItem();
        inline RootGraphicsItem *graphicsItem();

        inline QDomDocument document() const;
        inline QString filename() const;

        static QString getPath(const QString& filename);

        void load(const QString& filename);
        void fillSettings();

    private:
        XmlItem *init(const QDomElement& root, XmlItem *rootTreeItem);

        void processNode(const QDomElement& root, const GraphicsRegion::Data& data, XmlItem *rootTreeItem);
        void processLineNode(const QDomElement& root, XmlItem *rootTreeItem);
        void processTypoNode(const QDomElement& root, const QPoint& xPoint);

        RootGraphicsItem *graphicsItem_;
        XmlItem *xmlItem_;

        QDomDocument xml_;
        QString filename_;
};

inline XmlItem *Xml::xmlItem()
{ return xmlItem_; }

inline RootGraphicsItem *Xml::graphicsItem()
{ return graphicsItem_; }

inline QDomDocument Xml::document() const
{ return xml_; }

inline QString Xml::filename() const
{ return filename_; }

#endif // XML_H
