#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QScrollBar>

class GraphicsView :
        public QGraphicsView
{
        Q_OBJECT

    public:
        explicit GraphicsView(QWidget *parent = 0);
        explicit GraphicsView(QGraphicsScene *scene, QWidget *parent = 0);

    protected:
        void init();

        void resizeEvent(QResizeEvent *event);

        void wheelEvent(QWheelEvent *event);
        void keyPressEvent(QKeyEvent *event);
        void mousePressEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);

    private:
        bool isWidthGreater_;
        bool isContainedInView_;
        QSizeF scaleRatio_;

        bool isRightClicking_;
        QPointF prevMousePos_;

    public slots:
        void fitInView(const QRectF& rect);

    signals:
        void beginDrag();
        void endDrag();
};

#endif // GRAPHICSVIEW_H
