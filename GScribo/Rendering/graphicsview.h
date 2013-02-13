#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>

class GraphicsView :
        public QGraphicsView
{
    public:
        explicit GraphicsView(QWidget *parent = 0);
        explicit GraphicsView(QGraphicsScene *scene, QWidget *parent = 0);

        void fitInView(const QGraphicsItem *item, Qt::AspectRatioMode aspectRadioMode);
        void fitInView(const QRectF &rect, Qt::AspectRatioMode aspectRadioMode);
        void fitInView(qreal x, qreal y, qreal w, qreal h, Qt::AspectRatioMode aspectRadioMode);

    protected:
        void wheelEvent(QWheelEvent *event);
        void keyPressEvent(QKeyEvent *event);

    private:
        qreal scaleRatio_;
};

#endif // GRAPHICSVIEW_H
