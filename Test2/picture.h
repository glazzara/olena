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
        Picture(QTextEdit *textEdit);
        Picture(const QString &filename, QTextEdit *textEdit);
        void load(const QString &filename);

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    private:
        QString pressPosition();
        QString releasePosition();

        QTextEdit *textEdit;
        QPainter *painter;
        QPointF pressPos;
        QPointF releasePos;
        bool isPush;
};

#endif // PICTURE_H
