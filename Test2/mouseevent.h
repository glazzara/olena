#ifndef MOUSEEVENT_H
#define MOUSEEVENT_H

#include <QGraphicsView>

class MouseEvent :
        public QGraphicsView
{
:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // MOUSEEVENT_H
