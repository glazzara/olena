#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QGraphicsView>
#include <QMouseEvent>
#include "selectionitem.h"

class GraphicsView :
        public QGraphicsView
{
    public:
        GraphicsView();
        explicit GraphicsView(QWidget *parent);
        explicit GraphicsView(SelectionItem *selectionItem);
        explicit GraphicsView(QWidget *parent, SelectionItem *selectionItem);

    protected:
        void mousePressEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);

    private:
        QPointF pressPoint;
        QRectF selRect;
        bool isPressing;
        SelectionItem *selectionItem;
};

#endif // VIEWPORT_H
