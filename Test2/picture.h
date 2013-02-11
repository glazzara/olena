#ifndef PICTURE_H
#define PICTURE_H

#include <QGraphicsObject>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QRectF>
#include <QTextEdit>

class Picture :
        public QGraphicsPixmapItem
{
    public:
        Picture();
        void load(const QString &filename);

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    private:
        QPointF pressPos;
        QPointF releasePos;
        bool isPress;
};

#endif // PICTURE_H
