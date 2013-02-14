#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>

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
        inline void mousePressEvent(QMouseEvent *event);
        inline void mouseReleaseEvent(QMouseEvent *event);

    private:
        qreal scaleRatio_;

    public slots:
        void fitInView(const QGraphicsItem *item, Qt::AspectRatioMode aspectRadioMode = Qt::KeepAspectRatio);
        void fitInView(const QRectF &rect, Qt::AspectRatioMode aspectRadioMode = Qt::KeepAspectRatio);
        void fitInView(qreal x, qreal y, qreal w, qreal h, Qt::AspectRatioMode aspectRadioMode = Qt::KeepAspectRatio);
};

inline void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{ if(event->button() == Qt::RightButton) setCursor(QCursor(Qt::ArrowCursor)); QGraphicsView::mouseReleaseEvent(event); }

void GraphicsView::mousePressEvent(QMouseEvent *event)
{ if(event->buttons() == Qt::RightButton) setCursor(QCursor(Qt::ClosedHandCursor)); QGraphicsView::mousePressEvent(event); }

#endif // GRAPHICSVIEW_H
