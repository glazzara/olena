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

#include "image_region.hh"

ImageRegion::ImageRegion(region::RegionId id,
			 QString name,
			 QColor color,
			 QString attr_id,
			 const QVector<QPoint>& points,
			 bool outline,
			 bool fill,
			 bool precise,
			 bool draw)
  : QGraphicsItem(0),
    id_(id),
    name_(name),
    color_(color),
    attr_id_(attr_id),
    shape_(),
    rect_(),
    outline_(outline),
    precise_(precise),
    fill_(fill),
    draw_(draw),
    selected_(false),
    alpha_(50)
{
  setCursor(Qt::ArrowCursor);
  if (id_ == region::Paragraph)
    setZValue(2);
  else if (id_ == region::Line)
    setZValue(3);
  else
    setZValue(1);

  if (points.size() == 0)
    return;

  for (int i = 1; i < points.size(); ++i)
    shape_.lineTo(points[i] - points[0]);
  shape_.lineTo(QPoint(0, 0));

  rect_ = shape_.boundingRect();
  setPos(points[0] + QPointF(0.5, 0.5));

  prepareGeometryChange();
}

ImageRegion::~ImageRegion()
{
}

region::RegionId ImageRegion::id()
{
  return id_;
}

void
ImageRegion::paint(QPainter* painter,
		   const QStyleOptionGraphicsItem*,
		   QWidget*)
{
  if (!draw_ && !selected_)
    return;

  int width = 0;
  if (precise_)
    width = 1;

  if (outline_)
    painter->setPen(QPen(QBrush(color_), width, Qt::SolidLine,
			 Qt::SquareCap, Qt::MiterJoin));
  else
    painter->setPen(QColor(0, 0, 0, 0));

  QColor brush = color_;
  if (fill_)
    brush.setAlpha(alpha_);
  else
    brush.setAlpha(0);
  painter->setBrush(brush);

  painter->drawPath(shape_);

  if (selected_)
  {
    QPolygonF sceneRect = mapFromScene(scene()->sceneRect());
    QPainterPath path;
    path.addPolygon(sceneRect);
    QColor brush(255, 255, 255);
    brush.setAlpha(120);
    painter->setBrush(brush);
    painter->setPen(QColor(0, 0, 0, 0));
    painter->drawPath(path.subtracted(shape_));
  }
}

void
ImageRegion::select()
{
  if (!selected_)
  {
    selected_ = true;
    setZValue(4);
    prepareGeometryChange();
    scene()->invalidate();
    update();
  }
}

void
ImageRegion::deselect()
{
  if (selected_)
  {
    selected_ = false;
    if (id_ == region::Paragraph)
      setZValue(2);
    else if (id_ == region::Line)
      setZValue(3);
    else
      setZValue(1);
    prepareGeometryChange();
    scene()->invalidate();
    update();
  }
}
