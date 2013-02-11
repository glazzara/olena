#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QFileDialog>

#include "rootgraphicsitem.h"
#include "regionitem.h"
#include "selection.h"

class Scene :
        public QGraphicsScene
{
        Q_OBJECT

    public:
        explicit Scene(QObject *parent = 0);
        explicit Scene(const QRectF &sceneRect, QObject *parent = 0);
        explicit Scene(qreal x, qreal y, qreal width, qreal height, QObject *parent = 0);

        inline QString backgroundPath() const;
        inline QList<RegionItem *> selectedRegions() const;

        void setRoot(RootGraphicsItem *rootItem);
        inline RootGraphicsItem *root() const;

        void select(const QRectF& rect);
        void select(const QPointF& point);

        void changeScene(const QString& filename, RootGraphicsItem *root = 0);

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    private:
        void init();
        void select(QGraphicsItem *root, const QPointF& point, const QRectF& rect);

        QGraphicsView *mainView_;

        QString backgroundPath_;
        RootGraphicsItem *root_;

        Selection selection_;
        QList<RegionItem *> selectedRegions_;
        QList<RegionItem *> baseSelection_;

        QPointF pressPos_;
        bool isPressing_;

    public slots:
        void clear();
        void clearSelection();

        void select(const QList<RegionItem *>& selectedRegions);
        inline void selectBase();
        void unselect(const QList<RegionItem *>& unselectedItems);

        void setVisible(const GraphicsRegion::Id& region, bool visible);
        void ensureVisible(QGraphicsItem *graphicsItem);

    signals:
        void selectionCleared();
        void newSelection();
        void newSelection(const QList<RegionItem *>& selectedRegions);
};

inline QString Scene::backgroundPath() const
{ return backgroundPath_; }

inline RootGraphicsItem *Scene::root() const
{ return root_; }

inline QList<RegionItem *> Scene::selectedRegions() const
{ return selectedRegions_; }

inline void Scene::selectBase()
{ select(baseSelection_); }

inline void Scene::select(const QPointF& point)
{ select(root_, point, QRectF()); }

inline void Scene::select(const QRectF& rect)
{ select(root_, QPointF(), rect); }

#endif // SCENE_H
