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
    rootItem_ = 0;

    // Disable the scene size adaptation to items rect with a non null rect.
    setSceneRect(0, 0, 0, 1);

    selection_.setRect(0, 0, 0, 0);
    addItem(&selection_);
}

void Scene::clear()
{
    if(rootItem_)
        delete rootItem_;

    selectedItems_.clear();
    rootItem_ = 0;
}

void Scene::clearSelection()
{
    QGraphicsItem *child;
    PolygonItem *polygonItem;

    // Unselect all items.
    foreach(child, selectedItems_)
    {
        polygonItem = static_cast<PolygonItem *>(child);
        polygonItem->unselect();
    }

    selectedItems_.clear();
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && !isPressing_)
    {
        isPressing_ = true;

        pressPos_ = event->scenePos();
        selection_.show();
    }
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(isPressing_)
    {
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

          // Redraw all items in selection.
          if(rootItem_)
          {
              clearSelection();
              emit beginSelection();

              // Check if it is a click.
              if(selection_.rect().width() * selection_.rect().height() == 0)
                  selectItems(event->scenePos());
              else
                  selectItems(selection_.rect());

              emit endSelection(selectedItems_);
          }

          selection_.setRect(0, 0, 0, 0);
          selection_.hide();
      }
}

void Scene::selectItems(const QList<PolygonItem *>& selectedItems, bool addToSelection)
{
    if(!addToSelection)
        clearSelection();

    PolygonItem *child;
    foreach(child, selectedItems)
    {
        if(!child->isSelected())
        {
            child->select();
            selectedItems_ << child;
        }
    }
}

void Scene::selectItems(QGraphicsItem *root, const QPointF& point, const QRectF& rect)
{
    QGraphicsItem *child;
    PolygonItem *polygonItem;

    // Check if it is a click.
    if(!point.isNull())
    {
        foreach(child, root->childItems())
        {
            polygonItem = static_cast<PolygonItem *>(child);

            if(polygonItem->contains(point))
            {
                polygonItem->select();
                selectedItems_ << polygonItem;
            }
        }
    }

    else
    {
        foreach(child, root->childItems())
        {
            polygonItem = static_cast<PolygonItem *>(child);

            if(polygonItem->intersects(rect))
            {
                polygonItem->select();
                selectedItems_ << polygonItem;
            }
        }
    }
}

void Scene::setRootItem(RootGraphicsItem *rootItem)
{
    // Delete all items.
    clear();

    rootItem_ = rootItem;
    addItem(rootItem);
}

void Scene::changeScene(const QString& filename, RootGraphicsItem *rootItem)
{
    backgroundPath_ = filename;

    // Delete all items.
    clear();

    QPixmap pixmap(filename);
    setSceneRect(pixmap.rect());
    setBackgroundBrush(QBrush(pixmap));

    // Add new items.
    if(rootItem)
    {
        rootItem_ = rootItem;
        addItem(rootItem);
    }
}
