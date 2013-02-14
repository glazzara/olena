#include "graphicsview.h"
#include <QDebug>

GraphicsView::GraphicsView(QWidget *parent) :
        QGraphicsView(parent)
{
    init();
}

GraphicsView::GraphicsView(QGraphicsScene *scene, QWidget *parent) :
        QGraphicsView(scene, parent)
{
    init();
}

void GraphicsView::init()
{
    scaleRatio_ = 1;
    setTransformationAnchor(GraphicsView::AnchorUnderMouse);
    setBackgroundBrush(QBrush(Qt::lightGray));
}

void GraphicsView::wheelEvent(QWheelEvent *event)
{
    // Current wheel have an angle of 15 degrees by steps with a delta of + or - 120. A good scale is +- 0.1 in that case.
    // So we have to adapt for wheel whith better precision.

    qreal degrees = 1 + static_cast<qreal>(event->delta()) / 1200;
    //qDebug() << "Degrees : " << degrees;
    qreal nextScaleRatio = scaleRatio_ * degrees;
    //qDebug() << "Next Scale Ratio : " << nextScaleRatio;
    if(event->delta() > 0)
    {
        if(nextScaleRatio <= 12)
        {
            scaleRatio_ = nextScaleRatio;
            scale(degrees, degrees);
        }
    }
    else
    {
        if(nextScaleRatio > 1)
        {
            scaleRatio_ = nextScaleRatio;
            scale(degrees, degrees);
        }
        else if(scaleRatio_ != 1)
            fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
    }

    //qDebug() << "Scale Ratio : " << scaleRatio_;
}

void GraphicsView::keyPressEvent(QKeyEvent* event)
{
    // Bitwise AND to check if at least CTRL is pushed.
    if(event->modifiers() & Qt::ControlModifier)
    {
        if(event->key() == Qt::Key_Plus && scaleRatio_ <= 11.8)
        {
            scaleRatio_ *= 1.2;
            scale(1.2, 1.2);
        }

        else if(event->key() == Qt::Key_Minus && scaleRatio_ >= 1.2)
        {
            scaleRatio_ *= 0.8;
            scale(0.8, 0.8);
        }
    }

    QGraphicsView::keyPressEvent(event);
}

// When we fit an item or a rectangle in view (it's done each time a new picture is loaded), reinit the scale ratio.
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

void GraphicsView::resizeEvent(QResizeEvent *event)
{
    /*if(!isScalingScene)
    {
    qDebug() << event->oldSize();
    qDebug() << event->size();
    float ratio = static_cast<float>(event->size().width() * event->size().height()) / static_cast<float>(event->oldSize().width() * event->oldSize().height());

    qDebug() << "Scale Ratio / Ratio : " << scaleRatio_ / ratio;

    // Check if the new geometry area is greater than the scene rect.
    if(scaleRatio_ / ratio < 1)
        fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
    else
    {
        scaleRatio_ *= ratio;
        qDebug() << "Scale Ratio after minimi : " << scaleRatio_ * ratio;
    }

    if(scaleRatio_ / ratio < 1)
        fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
    }
    isScalingScene = false;*/

    //fitInView(scene()->sceneRect());
    QGraphicsView::resizeEvent(event);
}
