#include "xml.h"

static const GraphicRegion::Data itemsData[] =
{
    /*           COLOR               |             NAME             |               REGION                  | ZVALUE */
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

    load(filename);
}

void Xml::load(const QString &filename)
{
    if(!filename.isEmpty())
    {
        // Reset graphic and tree items;
        tItem = new QTreeWidgetItem;
        gItem = new QGraphicsPolygonItem;

        QFile xmlFile(filename);
        xmlFile.open(QIODevice::ReadOnly);

        // Fill new xml tree.
        xml.clear();
        xml.setContent(&xmlFile);

        xmlFile.close();

        QDomElement root = xml.documentElement();
        tItem->setText(0, root.tagName());
        tItem->setData(1, Qt::UserRole, VariantPointer<QDomNamedNodeMap>::toQVariant(new QDomNamedNodeMap(root.attributes())));

        root = root.firstChild().toElement();
        QTreeWidgetItem *parentTreeItem = init(root, tItem);
        parentTreeItem->setData(1, Qt::UserRole, VariantPointer<QDomNamedNodeMap>::toQVariant(0));

        // Run through the xml file structure by structure.
        root = root.nextSibling().firstChild().toElement();
        processNode(root, itemsData[1], parentTreeItem);
        for(int i = 2; i < 10; i++)
            processNode(root.nextSiblingElement(itemsData[i].name), itemsData[i], parentTreeItem);
    }
}

QTreeWidgetItem *Xml::init(const QDomElement& root, QTreeWidgetItem *rootTreeItem)
{
    QDomElement node = root;

    QTreeWidgetItem *parentTreeItem = new QTreeWidgetItem(rootTreeItem);
    parentTreeItem->setText(0, root.tagName());
    parentTreeItem->setData(0, Qt::UserRole, VariantPointer<QDomNamedNodeMap>::toQVariant(0));

    QString nodeText;
    QTreeWidgetItem *treeItem, *childTreeItem;

    // Run through "METADATA" node and subnodes.
    node = node.firstChild().toElement();
    while(!node.isNull())
    {
        treeItem = new QTreeWidgetItem(parentTreeItem, treeItem);
        treeItem->setText(0, node.tagName());
        // Store the xml node in the item.
        treeItem->setData(0, Qt::UserRole, VariantPointer<QDomNamedNodeMap>::toQVariant(0));

        // Add widget item child with text content.
        nodeText = node.text();
        childTreeItem = new QTreeWidgetItem(treeItem);
        childTreeItem->setText(0, nodeText);

        node = node.nextSibling().toElement();
    }

    // Add "PAGE" root node and return it.
    parentTreeItem = new QTreeWidgetItem(rootTreeItem, parentTreeItem);
    parentTreeItem->setText(0, root.nextSibling().toElement().tagName());
    parentTreeItem->setData(0, Qt::UserRole, VariantPointer<QDomNamedNodeMap>::toQVariant(new QDomNamedNodeMap(root.nextSibling().attributes())));

    return parentTreeItem;
}

void Xml::processNode(const QDomElement& root, const GraphicRegion::Data& data, QTreeWidgetItem *rootTreeItem)
{
    if(!root.isNull())
    {
        // Create corresponding tree item.
        QTreeWidgetItem *parentTreeItem = new QTreeWidgetItem(rootTreeItem, QStringList(root.tagName()));
        parentTreeItem->setData(0, Qt::UserRole, VariantPointer<QDomNamedNodeMap>::toQVariant(new QDomNamedNodeMap(root.attributes())));

        QPolygon polygon;

        // Run through all points data.
        QDomElement node = root.firstChild().firstChild().toElement();
        while(!node.isNull())
        {
            polygon << QPoint(node.attribute("x", "null").toInt(), node.attribute("y", "null").toInt());
            node = node.nextSibling().toElement();
        }

        // Create the graphical item from data structure.
        PolygonItem *polygonItem = new PolygonItem(polygon, gItem);
        polygonItem->loadData(data);

        // Store tree object in the graphical object an vice versa.
        polygonItem->setData(1, VariantPointer<QTreeWidgetItem>::toQVariant(parentTreeItem));
        parentTreeItem->setData(0, Qt::UserRole+1, VariantPointer<PolygonItem>::toQVariant(polygonItem));

        if(data.region == GraphicRegion::Text)
            processLineNode(root.firstChild().nextSiblingElement("Line"), polygonItem, parentTreeItem);

        // Run through all nodes from the same data structure recursively.
        processNode(root.nextSiblingElement(data.name), data, rootTreeItem);
    }
}

void Xml::processLineNode(const QDomElement& root, PolygonItem *parentPolygonItem, QTreeWidgetItem *rootTreeItem)
{
    if(!root.isNull())
    {
        // Create corresponding tree item.
        QTreeWidgetItem *parentTreeItem = new QTreeWidgetItem(rootTreeItem, QStringList(root.tagName()));
        parentTreeItem->setData(0, Qt::UserRole, VariantPointer<QDomNamedNodeMap>::toQVariant(new QDomNamedNodeMap(root.attributes())));

        QPolygon polygon;
        int x, y;

        int xMin = INT_MAX;
        int xMax = INT_MIN;

        // Run through all points data.
        QDomElement node = root.firstChild().firstChild().toElement();
        while(!node.isNull())
        {
            x = node.attribute("x", "null").toInt();
            y = node.attribute("y", "null").toInt();

            // Get beginning and end of line.
            if(xMin > x)
                xMin = x;
            if(xMax < x)
                xMax = x;

            polygon << QPoint(x, y);
            node = node.nextSibling().toElement();
        }

        // Create the graphical item from data structure.
        PolygonItem *polygonItem = new PolygonItem(polygon, parentPolygonItem);
        polygonItem->loadData(itemsData[0]);

        // Store tree object in the graphical object and vice versa.
        polygonItem->setData(1, VariantPointer<QTreeWidgetItem>::toQVariant(parentTreeItem));
        parentTreeItem->setData(0, Qt::UserRole+1, VariantPointer<PolygonItem>::toQVariant(polygonItem));

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
