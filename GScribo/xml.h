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

// WARNING : The order is VERY IMPORTANT (depending on the enum).
static const GraphicsRegion::Data datas_[] =
{
    /*           COLOR               |             NAME            |                 REGION                |              PARENT             |  Z */
    { QColor::fromRgb(0, 100, 0, 90),   "TextRegion",                   GraphicsRegion::TextRegion,             GraphicsRegion::Text,           1 },
    { QColor::fromRgb(255, 0, 0, 90),   "Line",                         GraphicsRegion::Line,                   GraphicsRegion::Text,           2 },
    { QColor::fromRgb(0, 0, 255, 90),   "VerticalSeparatorRegion",      GraphicsRegion::VerticalSeparator,      GraphicsRegion::Separators,     2 },
    { QColor::fromRgb(0, 0, 255, 90),   "HorizontalSeparatorRegion",    GraphicsRegion::HorizontalSeparator,    GraphicsRegion::Separators,     2 },
    { QColor::fromRgb(0, 0, 128, 90),   "WhitespaceSeparatorRegion",    GraphicsRegion::WhiteSpaceSeparator,    GraphicsRegion::Separators,     2 },
    { QColor::fromRgb(255, 120, 0, 90), "ImageRegion",                  GraphicsRegion::Image,                  GraphicsRegion::Miscellaneous,  1 },
    { QColor::fromRgb(43, 39, 128, 90), "NoiseRegion",                  GraphicsRegion::Noise,                  GraphicsRegion::Miscellaneous,  2 },
    { QColor::fromRgb(220, 246, 0, 90), "TableRegion",                  GraphicsRegion::Table,                  GraphicsRegion::Miscellaneous,  2 },
    { QColor::fromRgb(170, 0, 255, 90), "MathsRegion",                  GraphicsRegion::Maths,                  GraphicsRegion::Miscellaneous,  2 },
    { QColor::fromRgb(255, 0, 144, 90), "GraphicsRegion",               GraphicsRegion::Graphic,                GraphicsRegion::Miscellaneous,  2 },
    { QColor::fromRgb(0, 204, 255, 90), "ChartRegion",                  GraphicsRegion::Chart,                  GraphicsRegion::Miscellaneous,  2 },
    { QColor::fromRgb(128, 0, 255),     "Baseline",                     GraphicsRegion::Baseline,               GraphicsRegion::Typology,       3 },
    { QColor::fromRgb(128, 0, 255),     "Meanline",                     GraphicsRegion::Meanline,               GraphicsRegion::Typology,       3 },
    { QColor::fromRgb(255, 255, 255),   "Text",                         GraphicsRegion::Text,                   GraphicsRegion::None,           0 },
    { QColor::fromRgb(255, 255, 255),   "Separators",                   GraphicsRegion::Separators,             GraphicsRegion::None,           0 },
    { QColor::fromRgb(255, 255, 255),   "Miscellaneous",                GraphicsRegion::Miscellaneous,          GraphicsRegion::None,           0 },
    { QColor::fromRgb(255, 255, 255),   "Typological Lines",            GraphicsRegion::Typology,               GraphicsRegion::None,           0 }
};

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
        inline static GraphicsRegion::Data dataFromRegion(GraphicsRegion::Id region);

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

inline GraphicsRegion::Data Xml::dataFromRegion(GraphicsRegion::Id region)
{ return datas_[region]; }

#endif // XML_H
