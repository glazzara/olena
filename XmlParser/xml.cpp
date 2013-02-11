#include "xml.h"
#include "region.h"

static const GraphicRegion::Data itemsData[] =
{
    {QColor::fromRgb(0, 0, 255, 60), "VerticalSeparatorRegion", (int)GraphicRegion::VerticalSeparator, 2},
    {QColor::fromRgb(0, 0, 255, 60), "HorizontalSeparatorRegion", (int)GraphicRegion::HorizontalSeparator, 2},
    {QColor::fromRgb(0, 0, 128, 60), "WhitespaceSeparatorRegion", (int)GraphicRegion::WhiteSpaceSeparator, 2},
    {QColor::fromRgb(255, 120, 0, 60), "ImageRegion", (int)GraphicRegion::Image, 1},
    {QColor::fromRgb(43, 39, 128, 60), "NoiseRegion", (int)GraphicRegion::Noise, 2},
    {QColor::fromRgb(220, 246, 0, 60), "TableRegion", (int)GraphicRegion::Table, 2},
    {QColor::fromRgb(170, 0, 255, 60), "MathsRegion", (int)GraphicRegion::Maths, 3},
    {QColor::fromRgb(255, 0, 144, 60), "GraphicRegion", (int)GraphicRegion::Graphic, 2},
    {QColor::fromRgb(0, 204, 255, 60), "ChartRegion", (int)GraphicRegion::Chart, 2}
};

Xml::Xml()
{
}

void Xml::graphicsRegion(const QDomElement& element, const GraphicRegion::Data& data, Scene *scene)
{
    if(!element.isNull())
    {
        QPolygon polygon;
        QDomElement elt = element.firstChild().firstChild().toElement();
        while(!elt.isNull())
        {
            polygon << QPoint(elt.attribute("x", "null").toInt(), elt.attribute("y", "null").toInt());
            elt = elt.nextSibling().toElement();
        };
        PolygonItem *polygonRegion = new PolygonItem(polygon, 0, scene);
        polygonRegion->setData(0, QVariant::fromValue(data.region));
        polygonRegion->setColor(data.color);
        polygonRegion->setZValue(data.zValue);
        connect(scene, SIGNAL(repaintItems(QRectF,bool)), polygonRegion, SLOT(repaint(QRectF,bool)));
        graphicsRegion(element.nextSiblingElement(data.name), data, scene);
    }
}

void Xml::graphicsTypoRegion(const QDomElement& element, const QPoint& xPos, Scene *scene)
{
    int yPos = element.attribute("baseline", "null").toInt();
    QGraphicsLineItem *baselineRegion = new QGraphicsLineItem(QLine(QPoint(xPos.x(), yPos), QPoint(xPos.y(), yPos)), 0, scene);
    baselineRegion->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    baselineRegion->setData(0, QVariant::fromValue((int)GraphicRegion::Baseline));
    baselineRegion->setZValue(3);
    yPos = element.attribute("meanline", "null").toInt();
    QGraphicsLineItem *meanlineRegion = new QGraphicsLineItem(QLine(QPoint(xPos.x(), yPos), QPoint(xPos.y(), yPos)), 0, scene);
    meanlineRegion->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    meanlineRegion->setData(0, QVariant::fromValue((int)GraphicRegion::Meanline));
    meanlineRegion->setZValue(3);
}

void Xml::graphicsLineRegion(const QDomElement &element, Scene *scene)
{
    if(!element.isNull())
    {
        QPolygon polygonLine;
        QDomElement elt = element.firstChild().firstChild().toElement();
        int xMin = INT_MAX;
        int xMax = -1;
        int x, y;
        while(!elt.isNull())
        {
            x = elt.attribute("x", "null").toInt();
            y = elt.attribute("y", "null").toInt();
            if (xMin > x)
                xMin = x;
            if (xMax < x)
                xMax = x;
            polygonLine << QPoint(x, y);
            elt = elt.nextSibling().toElement();
        }
        PolygonItem *lineRegion = new PolygonItem(polygonLine, 0, scene);
        lineRegion->setData(0, QVariant::fromValue((int)GraphicRegion::Line));
        lineRegion->setColor(QColor::fromRgb(255, 0, 0, 80));
        lineRegion->setZValue(2);
        connect(scene, SIGNAL(repaintItems(QRectF,bool)), lineRegion, SLOT(repaint(QRectF,bool)));
        graphicsTypoRegion(element, QPoint(xMin, xMax), scene);
        graphicsLineRegion(element.nextSiblingElement("Line"), scene);
    }
}

void Xml::graphicsTextRegion(const QDomElement &element, Scene *scene)
{
    if(!element.isNull())
    {
        QPolygon polygonText;
        QDomElement elt = element.firstChild().firstChild().toElement();
        while(!elt.isNull())
        {
            polygonText << QPoint(elt.attribute("x", "null").toInt(), elt.attribute("y", "null").toInt());
            elt = elt.nextSibling().toElement();
        }
        PolygonItem *textRegion = new PolygonItem(polygonText, 0, scene);
        textRegion->setColor(QColor::fromRgb(0, 100, 0, 80));
        textRegion->setData(0, QVariant::fromValue((int)GraphicRegion::Text));
        textRegion->setZValue(1);
        connect(scene, SIGNAL(repaintItems(QRectF,bool)), textRegion, SLOT(repaint(QRectF,bool)));
        graphicsLineRegion(element.firstChild().nextSiblingElement("Line"), scene);
        graphicsTextRegion(element.nextSiblingElement("TextRegion"), scene);
    }
}

void Xml::parseItems(const QString &filename, Scene *scene)
{
    QFile xmlFile(filename);
    xmlFile.open(QIODevice::ReadOnly);
    QDomDocument xml;
    xml.setContent(&xmlFile);
    xmlFile.close();
    QDomElement element = xml.documentElement().firstChild().nextSiblingElement("Page").firstChild().toElement();
    graphicsTextRegion(element, scene);
    for(int i = 0; i < 9; i++)
        graphicsRegion(element.nextSiblingElement(itemsData[i].name), itemsData[i], scene);
}
