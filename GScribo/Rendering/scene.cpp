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
    isPressing_ = false;
    clic_ = false;
    item_ = 0;

    // Disable the scene size adaptation to items rect with a non null rect.
    setSceneRect(0, 0, 0, 1);

    selection_.setRect(0, 0, 0, 0);
    addItem(&selection_);
}

void Scene::clear()
{
    if(item_)
        delete item_;

    item_ = 0;
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && !isPressing_)
    {
        isPressing_ = true;
        clic_ = true;

        pressPos_ = event->scenePos();
        selection_.show();
    }
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(isPressing_)
    {
        clic_ = false;

        // Adapt if the selection rectangle has a negative size.
        if(pressPos_.x() < event->scenePos().x())
            selection_.setRect(pressPos_.x(), 0, event->scenePos().x()-pressPos_.x(), 0);
        else
            selection_.setRect(event->scenePos().x(), 0, pressPos_.x()-event->scenePos().x(), 0);

        if(pressPos_.y() < event->scenePos().y())
            selection_.setRect(selection_.rect().x(), pressPos_.y(), selection_.rect().width(), event->scenePos().y()-pressPos_.y());
        else
            selection_.setRect(selection_.rect().x(), event->scenePos().y(), selection_.rect().width(), pressPos_.y()-event->scenePos().y());
    }
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
      {
          isPressing_ = false;

          // If no selection, store the click position in the selection rectangle.
          if(clic_)
              selection_.setRect(QRectF(QPointF(0, 0), event->scenePos()));

          // Redraw all items in selection.
          if(item_)
          {
              emit beginSelection();
              selectItems(selection_.rect(), clic_);
              emit endSelection();
          }

          selection_.setRect(0, 0, 0, 0);
          selection_.hide();
      }
}

void Scene::selectItem(PolygonItem *graphicalItem)
{
    QGraphicsItem *child;
    PolygonItem *polygonItem;

    // Unselect all items.
    foreach(child, item_->childItems())
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

void Scene::selectItems(const QRectF& rect, bool clic)
{
    QGraphicsItem *child;
    foreach(child, item_->childItems())
        static_cast<PolygonItem *>(child)->setSelected(rect, clic);
}

void Scene::addPolygonItem(QGraphicsItem *item)
{
    // Delete all items in the scene.
    clear();

    item_ = item;
    addItem(item);
}

void Scene::changeScene(const QString& filename, const QPixmap& pixmap, QGraphicsItem *item)
{
    backgroundPath_ = filename;

    // Delete all items in the scene.
    clear();

    setSceneRect(pixmap.rect());
    setBackgroundBrush(QBrush(pixmap));

    // Add new items.
    if(item)
        addPolygonItem(item);
}
