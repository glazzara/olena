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
    selection = new Selection(this);
}        

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && !isPressing)
    {
        isPressing = true;
        clic = true;
        pressPos = event->scenePos();
    }
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(isPressing)
    {
        clic = false;
        if(pressPos.x() < event->scenePos().x())
            selection->setRect(pressPos.x(), 0, event->scenePos().x()-pressPos.x(), 0);
        else
            selection->setRect(event->scenePos().x(), 0, pressPos.x()-event->scenePos().x(), 0);
        if(pressPos.y() < event->scenePos().y())
            selection->setRect(selection->rect().x(), pressPos.y(), selection->rect().width(), event->scenePos().y()-pressPos.y());
        else
            selection->setRect(selection->rect().x(), event->scenePos().y(), selection->rect().width(), pressPos.y()-event->scenePos().y());
    }
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
      {
          isPressing = false;
          if(clic)
              emit repaintItems(QRectF(QPointF(0, 0), event->scenePos()), true);
          else
              emit repaintItems(selection->rect(), false);
          selection->setRect(0, 0, 0, 0);
      }
}

QString Scene::debug(QRect &rect)
{
    return "Position : " + QString::number(rect.x()) + ", " + QString::number(rect.y()) + "\n" +
           "Size : " + QString::number(rect.width()) + ", " + QString::number(rect.height());
}
