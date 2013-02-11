#ifndef SELECTION_H
#define SELECTION_H

#include <QGraphicsRectItem>
#include <QPen>
#include <climits>

class Selection :
        public QGraphicsRectItem
{
    public:
        explicit Selection(QGraphicsScene *scene = 0);
        explicit Selection(const QRectF& rect, QGraphicsScene *scene = 0);
        explicit Selection(qreal x, qreal y, qreal width, qreal height, QGraphicsScene *scene = 0);

    private:
        void init();
};

#endif // SELECTION_H
