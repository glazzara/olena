#include "xml.h"
#include "region.h"

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
    baselineRegion->setData(0, QVariant::fromValue((int)GraphicRegion::Baseline));
    baselineRegion->setZValue(3);
    yPos = element.attribute("meanline", "null").toInt();
    QGraphicsLineItem *meanlineRegion = new QGraphicsLineItem(QLine(QPoint(xPos.x(), yPos), QPoint(xPos.y(), yPos)), 0, scene);
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
    QVector<QGraphicsItem *> items;
    QDomElement element = xml.documentElement().firstChild().nextSiblingElement("Page").firstChild().toElement();
    graphicsTextRegion(element, scene);                                                         // TextRegion + LineRegion + Baseline + Meanline
    GraphicRegion::Data verticalData;
    verticalData.color = QColor::fromRgb(0, 0, 255, 80);
    verticalData.name = "VerticalSeparatorRegion";
    verticalData.region = (int)GraphicRegion::VerticalSeparator;
    verticalData.zValue = 2;
    graphicsRegion(element.nextSiblingElement(verticalData.name), verticalData, scene);         // VerticalRegion
    GraphicRegion::Data horizontalData;
    horizontalData.color = QColor::fromRgb(0, 0, 255, 80);
    horizontalData.name = "HorizontalSeparatorRegion";
    horizontalData.region = (int)GraphicRegion::HorizontalSeparator;
    horizontalData.zValue = 2;
    graphicsRegion(element.nextSiblingElement(horizontalData.name), horizontalData, scene);     // HorizontalRegion
    GraphicRegion::Data spaceData;
    spaceData.color = QColor::fromRgb(0, 0, 128, 80);
    spaceData.name = "WhitespaceSeparatorRegion";
    spaceData.region = (int)GraphicRegion::WhiteSpaceSeparator;
    spaceData.zValue = 2;
    graphicsRegion(element.nextSiblingElement(spaceData.name), spaceData, scene);               // WhitespaceRegion
    GraphicRegion::Data imageData;
    imageData.color = QColor::fromRgb(255, 120, 0, 80);
    imageData.name = "ImageRegion";
    imageData.region = (int)GraphicRegion::Image;
    imageData.zValue = 1;
    graphicsRegion(element.nextSiblingElement(imageData.name), imageData, scene);               // ImageRegion
    GraphicRegion::Data noiseData;
    noiseData.color = QColor::fromRgb(43, 39, 128, 80);
    noiseData.name = "NoiseRegion";
    noiseData.region = (int)GraphicRegion::Noise;
    noiseData.zValue = 2;
    graphicsRegion(element.nextSiblingElement(noiseData.name), noiseData, scene);               // NoiseRegion
    GraphicRegion::Data tableData;
    tableData.color = QColor::fromRgb(220, 246, 0, 80);
    tableData.name = "TableRegion";
    tableData.region = (int)GraphicRegion::Table;
    tableData.zValue = 2;
    graphicsRegion(element.nextSiblingElement(tableData.name), tableData, scene);               // TableRegion
    GraphicRegion::Data mathsData;
    mathsData.color = QColor::fromRgb(170, 0, 255, 80);
    mathsData.name = "MathsRegion";
    mathsData.region = (int)GraphicRegion::Maths;
    mathsData.zValue = 3;
    graphicsRegion(element.nextSiblingElement(mathsData.name), mathsData, scene);               // MathsRegion
    GraphicRegion::Data graphicData;
    graphicData.color = QColor::fromRgb(255, 0, 144, 80);
    graphicData.name = "GraphicRegion";
    graphicData.region = (int)GraphicRegion::Graphic;
    graphicData.zValue = 2;
    graphicsRegion(element.nextSiblingElement(graphicData.name), graphicData, scene);           // GraphicRegion
    GraphicRegion::Data chartData;
    chartData.color = QColor::fromRgb(0, 204, 255, 80);
    chartData.name = "ChartRegion";
    chartData.region = (int)GraphicRegion::Chart;
    chartData.zValue = 2;
    graphicsRegion(element.nextSiblingElement(chartData.name), chartData, scene);               // ChartRegion
}
