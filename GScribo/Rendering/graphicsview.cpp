#include "graphicsview.h"

GraphicsView::GraphicsView(QWidget *parent) :
        QGraphicsView(parent)
{
    scaleRatio_ = 1;
}

GraphicsView::GraphicsView(QGraphicsScene *scene, QWidget *parent) :
        QGraphicsView(scene, parent)
{
    scaleRatio_ = 1;
}

void GraphicsView::wheelEvent(QWheelEvent *event)
{
    // Current wheel have an angle of 15 degrees by steps with a delta of + or - 120. A good scale is +- 0.1 in that case.
    // So we have to adapt for wheel whith better precision.

    if((event->delta() > 0 && scaleRatio_ < 1.6) || (event->delta() < 0 && scaleRatio_ > 1))
    {
        qreal degrees = static_cast<qreal>(event->delta()) / 1200;
        scaleRatio_ += degrees;
        degrees += 1;
        scale(degrees, degrees);
    }

    QGraphicsView::wheelEvent(event);
}

void GraphicsView::keyPressEvent(QKeyEvent* event)
{
    // Bitwise AND to check if at least CTRL is pushed.
    if(event->modifiers() & Qt::ControlModifier)
    {
        if(event->key() == Qt::Key_Plus && scaleRatio_ < 1.5)
        {
            scaleRatio_ += 0.2;
            scale(1.2, 1.2);
        }

        else if(event->key() == Qt::Key_Minus && scaleRatio_ > 1)
        {
            scaleRatio_ -= 0.2;
            scale(0.8, 0.8);
        }
    }

    QGraphicsView::keyPressEvent(event);
}

void GraphicsView::fitInView(const QGraphicsItem *item, Qt::AspectRatioMode aspectRadioMode)
{
    scaleRatio_ = 1;
    QGraphicsView::fitInView(item, aspectRadioMode);
}

void GraphicsView::fitInView(const QRectF& rect, Qt::AspectRatioMode aspectRadioMode)
{
    scaleRatio_ = 1;
    QGraphicsView::fitInView(rect, aspectRadioMode);
}

void GraphicsView::fitInView(qreal x, qreal y, qreal w, qreal h, Qt::AspectRatioMode aspectRadioMode)
{
    scaleRatio_ = 1;
    QGraphicsView::fitInView(x, y, w, h, aspectRadioMode);
}
