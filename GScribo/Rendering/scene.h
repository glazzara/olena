#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QFileDialog>

#include "selection.h"
#include "polygonitem.h"

class Scene :
        public QGraphicsScene
{
        Q_OBJECT

    public:
        explicit Scene(QObject *parent = 0);
        explicit Scene(const QRectF &sceneRect, QObject *parent = 0);
        explicit Scene(qreal x, qreal y, qreal width, qreal height, QObject *parent = 0);

        inline QString backgroundPath() const;
        void setRootItem(QGraphicsItem *graphicalItem);
        inline QGraphicsItem *rootItem() const;

        void clear();
        void changeScene(const QString& filename, QGraphicsItem *rootItem = 0);
        void selectItems(const QRectF& rect, bool clic);

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    private:
        void init();

        QGraphicsItem *rootItem_;
        Selection selection_;
        QPointF pressPos_;
        bool isPressing_;
        bool click_;
        QString backgroundPath_;

    public slots:
        void selectItem(PolygonItem *graphicalItem);

    signals:
        void beginSelection();
        void endSelection();
};

inline QString Scene::backgroundPath() const
{ return backgroundPath_; }

inline QGraphicsItem *Scene::rootItem() const
{ return rootItem_; }

#endif // SCENE_H
