#include "scene.h"

Scene::Scene(QObject *parent):
    QGraphicsScene(parent)
{
    init();
}

Scene::Scene(const QRectF &sceneRect, QObject *parent):
    QGraphicsScene(sceneRect, parent)
{
    init();
}

Scene::Scene(qreal x, qreal y, qreal width, qreal height, QObject *parent):
    QGraphicsScene(x, y, width, height, parent)
{
    init();
}

void Scene::init()
{
    isPressing = false;
    clic = false;
    item = 0;

    // Disable the scene size adaptation to items rect with a non null rect.
    setSceneRect(0, 0, 0, 1);

    selection.setRect(0, 0, 0, 0);
    addItem(&selection);
}

void Scene::clear()
{
    if(item)
        delete item;

    item = 0;
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && !isPressing)
    {
        isPressing = true;
        clic = true;

        pressPos = event->scenePos();
        selection.show();
    }
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(isPressing)
    {
        clic = false;

        // Adapt if the selection rectangle has a negative size.
        if(pressPos.x() < event->scenePos().x())
            selection.setRect(pressPos.x(), 0, event->scenePos().x()-pressPos.x(), 0);
        else
            selection.setRect(event->scenePos().x(), 0, pressPos.x()-event->scenePos().x(), 0);

        if(pressPos.y() < event->scenePos().y())
            selection.setRect(selection.rect().x(), pressPos.y(), selection.rect().width(), event->scenePos().y()-pressPos.y());
        else
            selection.setRect(selection.rect().x(), event->scenePos().y(), selection.rect().width(), pressPos.y()-event->scenePos().y());
    }
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
      {
          isPressing = false;

          // If no selection, store the click position in the selection rectangle.
          if(clic)
              selection.setRect(QRectF(QPointF(0, 0), event->scenePos()));

          // Redraw all items in selection.
          if(item)
              selectItems(selection.rect(), clic);

          selection.setRect(0, 0, 0, 0);
          selection.hide();
      }
}

void Scene::selectItem(PolygonItem *graphicalItem)
{
    if(graphicalItem)
    {
        QGraphicsItem *child;
        PolygonItem *polygonItem;

        // Unselect all items.
        foreach(child, item->childItems())
        {
            polygonItem = static_cast<PolygonItem *>(child);
            polygonItem->unselect();

            // Unselect lines.
            foreach(child, polygonItem->childItems())
            {
                polygonItem = static_cast<PolygonItem *>(child);
                polygonItem->unselect();
            }
        }

        // Select the good item.
        graphicalItem->select();
        views()[0]->centerOn(graphicalItem);
    }
}

void Scene::selectItems(const QRectF &rect, bool clic)
{
    QGraphicsItem *child;
    PolygonItem *polygonItem;
    QList<QTreeWidgetItem *> selectionTree;
    QTreeWidgetItem *treeItem;
    bool isSel;

    // Redraw all items in the scene except selection.
    foreach(child, item->childItems())
    {
        polygonItem = static_cast<PolygonItem *>(child);
        isSel = polygonItem->isSelected(rect, clic);

        // If item selectionned, select it on the xml tree and the scene.
        if(isSel)
        {
            polygonItem->select();
            treeItem = VariantPointer<QTreeWidgetItem>::fromQVariant(polygonItem->data(1));
            treeItem = new QTreeWidgetItem(*treeItem);
            selectionTree << treeItem;
        }
        else
            polygonItem->unselect();

        if(polygonItem->data(0).toInt() == GraphicRegion::Text)
        {
            // Run through each child lines items and do the same.
            foreach(child, polygonItem->childItems())
            {
                polygonItem = static_cast<PolygonItem *>(child);
                isSel = polygonItem->isSelected(rect, clic);

                if(isSel)
                {
                    polygonItem->select();

                    // Check if the corresponding text region has been clicked.
                    if(treeItem)
                        treeItem->addChild(new QTreeWidgetItem(*VariantPointer<QTreeWidgetItem>::fromQVariant(polygonItem->data(1))));
                    else
                        selectionTree << new QTreeWidgetItem(*VariantPointer<QTreeWidgetItem>::fromQVariant(polygonItem->data(1)));
                }
                else
                    polygonItem->unselect();
            }
        }

        treeItem = 0;
    }

    emit selectTreeItems(selectionTree);
}

void Scene::addPolygonItem(QGraphicsItem *item)
{
    // Delete all items in the scene.
    clear();

    this->item = item;
    addItem(item);
}

void Scene::changeScene(const QString& filename, const QPixmap& pixmap, QGraphicsItem *item)
{
   path = filename;

    // Delete all items in the scene.
    clear();

    setSceneRect(pixmap.rect());
    setBackgroundBrush(QBrush(pixmap));

    // Add new items.
    if(item)
        addPolygonItem(item);
}
