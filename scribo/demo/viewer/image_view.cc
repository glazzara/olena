// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#include "image_view.hh"

ImageView::ImageView(QGraphicsScene* scene)
  : QGraphicsView(scene)
{
}

void
ImageView::wheelEvent(QWheelEvent* event)
{
  // Basic zoom code (same as yavgui)
  if (event->delta() != 0)
  {
    qreal degrees = ((qreal) event->delta()) / 8;
    qreal sc;
    if (degrees > 0)
      sc = 1 + degrees / 100;
    else
      sc = 1 / (1 - degrees / 100);

    QPointF center = mapToScene(width() / 2, height() / 2);
    QPointF mouse = mapToScene(event->pos());

    qreal dx = (mouse.x() - center.x());
    qreal dy = (mouse.y() - center.y());
    QPointF newCenter = QPointF (mouse.x() - dx / sc,
				 mouse.y() - dy / sc);
    scale(sc, sc);
    scaleUpdate();
  }
}

void
ImageView::keyPressEvent(QKeyEvent* event)
{
  if (event->key() == Qt::Key_PageUp)
    scale(1.25, 1.25);
  else if (event->key() == Qt::Key_PageDown)
    scale(0.75, 0.75);
  else if (event->key() == Qt::Key_Home)
    resetMatrix();
  else if (event->key() == Qt::Key_End)
    fitInView(sceneRect(), Qt::KeepAspectRatio);
  else
  {
    QGraphicsView::keyPressEvent(event);
    return;
  }
  scaleUpdate();
  event->accept();
}

void
ImageView::scaleUpdate()
{
  // Used to determine whether to change the main image cache mode.
  QRect orig(0, 0, 10, 1);
  QRectF scene = mapToScene(orig).boundingRect();
  emit scaleUpdated(10 / scene.width());
}

ImageView::~ImageView()
{
}
