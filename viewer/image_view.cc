//
// Document layout viewer.
//
// Copyright (C) 2009 Florent D'Halluin.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//

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
  }
}

ImageView::~ImageView()
{
}
