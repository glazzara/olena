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
    isRightClicking_ = false;
    scaleRatio_ = QSizeF(1, 1);
    setTransformationAnchor(GraphicsView::AnchorUnderMouse);
    setBackgroundBrush(QBrush(Qt::lightGray));

    // Important to hide scroll bars : when they appear, it triggers the resizeEvent.
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        emit beginDrag();
        setDragMode(QGraphicsView::ScrollHandDrag);
    }

    QMouseEvent *mouseEvent = new QMouseEvent(QEvent::MouseButtonPress, event->pos(), Qt::LeftButton,
                                              Qt::LeftButton, Qt::MetaModifier);
    event->ignore();
    QGraphicsView::mousePressEvent(mouseEvent);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        setDragMode(GraphicsView::NoDrag);
        emit endDrag();
    }

    QGraphicsView::mouseReleaseEvent(event);
}

void GraphicsView::wheelEvent(QWheelEvent *event)
{
    if(event->modifiers() & Qt::ControlModifier)
    {
        // Current wheel have an angle of 15 degrees by steps with a delta of + or - 120. A good scale is +- 0.1 in that case.
        // So we have to adapt for wheel whith better precision.
        qreal degrees = 1 + static_cast<qreal>(event->delta()) / 1200;
        qreal nextScaleRatio = degrees * isWidthGreater_ ? scaleRatio_.width() :
                                                           scaleRatio_.height();
        if(event->delta() > 0)
        {
            if(nextScaleRatio <= 15)
            {
                scaleRatio_ *= degrees;
                scale(degrees, degrees);
            }
        }
        else
        {
            if(nextScaleRatio >= 1.1)
            {
                scaleRatio_ *= degrees;
                scale(degrees, degrees);
            }
            else if(isContainedInView_)
            {
                QGraphicsView::fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
                scaleRatio_ = QSizeF(1, 1);
            }
        }
    }
    else
        QGraphicsView::wheelEvent(event);
}

void GraphicsView::keyPressEvent(QKeyEvent* event)
{
    // Bitwise AND to check if at least CTRL is pushed.
    if(event->modifiers() & Qt::ControlModifier)
    {
        qreal nextScaleRatio;
        if(event->key() == Qt::Key_Plus)
        {
            nextScaleRatio = 1.2 * isWidthGreater_ ? scaleRatio_.width() :
                                                     scaleRatio_.height();
            if(nextScaleRatio <= 15)
            {
                scaleRatio_ *= 1.2;
                scale(1.2, 1.2);
            }
        }
        else if(event->key() == Qt::Key_Minus)
        {
            nextScaleRatio = 0.8 * isWidthGreater_ ? scaleRatio_.width() :
                                                     scaleRatio_.height();
            if(nextScaleRatio >= 1.2)
            {
                scaleRatio_ *= 0.8;
                scale(0.8, 0.8);
            }
            else if(isContainedInView_)
            {
                QGraphicsView::fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
                scaleRatio_ = QSizeF(1, 1);
            }
        }
    }

    QGraphicsView::keyPressEvent(event);
}

void GraphicsView::fitInView(const QRectF& rect)
{
    setFocus();
    isWidthGreater_ = static_cast<qreal>(rect.width()) / static_cast<qreal>(rect.height()) > 1 ? true :
                                                                                                 false;
    qreal ratio = isWidthGreater_ ? static_cast<qreal>(width()) / static_cast<qreal>(rect.width()) :
                               static_cast<qreal>(height()) / static_cast<qreal>(rect.height());

    isContainedInView_ = ratio < 1 ? true : false;
    if(isContainedInView_)
    {
        scaleRatio_ = QSizeF(1, 1);
        QGraphicsView::fitInView(rect, Qt::KeepAspectRatio);
    }
}

void GraphicsView::resizeEvent(QResizeEvent *event)
{
    if(scene()->items().count() > 1)
    {
        qreal ratio = isWidthGreater_ ? static_cast<qreal>(event->size().width()) / static_cast<qreal>(event->oldSize().width()) :
                                        static_cast<qreal>(event->size().height()) / static_cast<qreal>(event->oldSize().height());
        scale(ratio, ratio);
    }

    QGraphicsView::resizeEvent(event);
}
