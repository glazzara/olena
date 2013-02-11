#include "xml.h"

static const GraphicRegion::Data itemsData[] =
{
    /*           COLOR               |             NAME             |               REGION                | ZVALUE */
    { QColor::fromRgb(255, 0, 0, 90),   "Line",                      (int)GraphicRegion::Line,                2 },
    { QColor::fromRgb(0, 100, 0, 90),   "TextRegion",                (int)GraphicRegion::Text,                1 },
    { QColor::fromRgb(0, 0, 255, 90),   "VerticalSeparatorRegion",   (int)GraphicRegion::VerticalSeparator,   2 },
    { QColor::fromRgb(0, 0, 255, 90),   "HorizontalSeparatorRegion", (int)GraphicRegion::HorizontalSeparator, 2 },
    { QColor::fromRgb(0, 0, 128, 90),   "WhitespaceSeparatorRegion", (int)GraphicRegion::WhiteSpaceSeparator, 2 },
    { QColor::fromRgb(255, 120, 0, 90), "ImageRegion",               (int)GraphicRegion::Image,               1 },
    { QColor::fromRgb(43, 39, 128, 90), "NoiseRegion",               (int)GraphicRegion::Noise,               2 },
    { QColor::fromRgb(220, 246, 0, 90), "TableRegion",               (int)GraphicRegion::Table,               2 },
    { QColor::fromRgb(170, 0, 255, 90), "MathsRegion",               (int)GraphicRegion::Maths,               2 },
    { QColor::fromRgb(255, 0, 144, 90), "GraphicRegion",             (int)GraphicRegion::Graphic,             2 },
    { QColor::fromRgb(0, 204, 255, 90), "ChartRegion",               (int)GraphicRegion::Chart,               2 }
};

Xml::Xml(const QString &filename)
{
    gItem = 0;
    tItem = 0;

    path = filename;
    load(filename);
}

QString Xml::getPath(const QString &filename)
{
    // Get instance of the configuration settings.
    Configs *const conf = Configs::getInstance();

    QString xmlPath = filename;

    // Get xml filename from image path.
    xmlPath.remove(0, xmlPath.lastIndexOf('/')+1);
    int pos = xmlPath.lastIndexOf('.');
    xmlPath.remove(pos, xmlPath.length()-pos);
    xmlPath += "_gui.xml";

    return conf->generalSaveXmlCustomDirPath() + "/" + xmlPath;
}

void Xml::load(const QString &filename)
{
    if(!filename.isEmpty())
    {
        // Reset graphic and tree items;
        tItem = new XmlItem;
        gItem = new QGraphicsPolygonItem;

        QFile xmlFile(filename);
        xmlFile.open(QIODevice::ReadOnly);

        // Fill new xml tree.
        xml.clear();
        xml.setContent(&xmlFile);

        xmlFile.close();

        QDomElement root = xml.documentElement();
        tItem->load(root);

        root = root.firstChild().toElement();
        XmlItem *parentTreeItem = init(root, tItem);

        // Run through the xml file structure by structure.
        root = root.nextSibling().firstChild().toElement();
        processNode(root, itemsData[1], parentTreeItem);
        for(int i = 2; i < 10; i++)
            processNode(root.nextSiblingElement(itemsData[i].name), itemsData[i], parentTreeItem);
    }
}

XmlItem *Xml::init(const QDomElement& root, XmlItem *rootTreeItem)
{
    QDomElement node = root;

    XmlItem *parentTreeItem = new XmlItem(node, rootTreeItem);

    QString nodeText;
    XmlItem *treeItem = 0;

    // Run through "METADATA" node and subnodes.
    node = node.firstChild().toElement();
    while(!node.isNull())
    {
        treeItem = new XmlItem(node, parentTreeItem, treeItem);
        node = node.nextSibling().toElement();
    }

    // Add "PAGE" root node and return it.
    parentTreeItem = new XmlItem(root.nextSibling().toElement(), rootTreeItem, parentTreeItem);
    return parentTreeItem;
}

void Xml::processNode(const QDomElement& root, const GraphicRegion::Data& data, XmlItem *rootTreeItem)
{
    if(!root.isNull())
    {
        // Create corresponding tree item.
        XmlItem *parentTreeItem = new XmlItem(root, rootTreeItem);
        XmlItem *parentItem = new XmlItem(root.firstChild().toElement(), parentTreeItem);

        QPolygon polygon;
        QString x, y;
        XmlItem *item = 0;

        // Run through all points data.
        QDomElement node = root.firstChild().firstChild().toElement();
        while(!node.isNull())
        {
            x = node.attribute("x", "null");
            y = node.attribute("y", "null");

            item = new XmlItem(node, parentItem, item);

            polygon << QPoint(x.toInt(), y.toInt());
            node = node.nextSibling().toElement();
        }

        // Create the graphical item from data structure.
        PolygonItem *polygonItem = new PolygonItem(polygon, gItem);
        polygonItem->loadData(data);
        parentTreeItem->setGraphicalItem(polygonItem);

        if(data.region == GraphicRegion::Text)
            processLineNode(root.firstChild().nextSiblingElement("Line"), polygonItem, parentTreeItem);

        // Run through all nodes from the same data structure recursively.
        processNode(root.nextSiblingElement(data.name), data, rootTreeItem);
    }
}

void Xml::processLineNode(const QDomElement& root, PolygonItem *parentPolygonItem, XmlItem *rootTreeItem)
{
    if(!root.isNull())
    {
        // Create corresponding tree item.
        XmlItem *parentTreeItem = new XmlItem(root, rootTreeItem, rootTreeItem->child(0));
        XmlItem *parentItem = new XmlItem(root.firstChild().toElement(), parentTreeItem);

        XmlItem *item = 0;
        QPolygon polygon;
        QString x, y;

        int xMin = INT_MAX;
        int xMax = INT_MIN;

        // Run through all points data.
        QDomElement node = root.firstChild().firstChild().toElement();
        while(!node.isNull())
        {
            x = node.attribute("x", "null");
            y = node.attribute("y", "null");

            item = new XmlItem(node, parentItem, item);

            // Get beginning and end of line.
            if(xMin > x.toInt())
                xMin = x.toInt();
            if(xMax < x.toInt())
                xMax = x.toInt();

            polygon << QPoint(x.toInt(), y.toInt());
            node = node.nextSibling().toElement();
        }

        // Create the graphical item from data structure.
        PolygonItem *polygonItem = new PolygonItem(polygon, parentPolygonItem);
        polygonItem->loadData(itemsData[0]);
        parentTreeItem->setGraphicalItem(polygonItem);

        // Get meanline and baseline.
        processTypoNode(root, QPoint(xMin, xMax), polygonItem);
        // Run through all line nodes recursively.
        processLineNode(root.nextSiblingElement("Line"), parentPolygonItem, rootTreeItem);
    }
}

void Xml::processTypoNode(const QDomElement& root, const QPoint& xPoint, PolygonItem *parentPolygonItem)
{
    int yPos = root.attribute("baseline", "null").toInt();
    QGraphicsLineItem *baselineItem = new QGraphicsLineItem(QLine(QPoint(xPoint.x(), yPos), QPoint(xPoint.y(), yPos)), parentPolygonItem);
    baselineItem->setData(0, QVariant::fromValue((int)GraphicRegion::Baseline));
    baselineItem->setZValue(3);

    yPos = root.attribute("meanline", "null").toInt();
    QGraphicsLineItem *meanlineItem = new QGraphicsLineItem(QLine(QPoint(xPoint.x(), yPos), QPoint(xPoint.y(), yPos)), parentPolygonItem);
    meanlineItem->setData(0, QVariant::fromValue((int)GraphicRegion::Meanline));
    meanlineItem->setZValue(3);
}
