#include "graphicsview.h"

GraphicsView::GraphicsView()
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    setScene(scene);
    isPressing = false;
    selRect.setRect(0, 0, 0, 0);
}

GraphicsView::GraphicsView(QWidget *parent)
{
    isPressing = false;
    QGraphicsScene *scene = new QGraphicsScene(this);
    setScene(scene);
    setParent(parent);
    selRect.setRect(0, 0, 0, 0);
    setGeometry(0, 0, parent->width(), parent->height());
}

GraphicsView::GraphicsView(SelectionItem *selectionItem)
{
    isPressing = false;
    QGraphicsScene *scene = new QGraphicsScene(this);
    setScene(scene);
    selRect.setRect(0, 0, 0, 0);
    this->selectionItem = selectionItem;
    scene->addItem(selectionItem);
}

GraphicsView::GraphicsView(QWidget *parent, SelectionItem *selectionItem)
{
    isPressing = false;
    QGraphicsScene *scene = new QGraphicsScene(this);
    setScene(scene);
    setParent(parent);
    selRect.setRect(0, 0, 0, 0);
    setGeometry(0, 0, parent->width(), parent->height());
    this->selectionItem = selectionItem;
    scene->addItem(selectionItem);
}

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && !isPressing)
    {
        isPressing = true;
        pressPoint = event->posF();
        selRect.setTopLeft(mapFromScene(event->posF()));
    }
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if(isPressing)
    {
        selRect.setBottomRight(mapFromScene(event->posF()));
        selectionItem->changeShape(selRect.normalized());
    }
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        isPressing = false;
        selRect.setBottomRight(mapFromScene(event->posF()));
        selectionItem->changeShape(selRect.normalized());
    }
}
