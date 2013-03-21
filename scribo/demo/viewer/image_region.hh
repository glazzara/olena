// Copyright (C) 2010, 2013 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef SCRIBO_DEMO_VIEWER_IMAGE_REGION_HH
# define SCRIBO_DEMO_VIEWER_IMAGE_REGION_HH

# include <QtGui>
# include "common.hh"

class ImageRegion
  : public QObject, public QGraphicsItem
{
  Q_OBJECT;
  Q_INTERFACES(QGraphicsItem);

public:
  ImageRegion(region::RegionId id,
	      QString name,
	      QColor color,
	      QString attr_id,
	      const QVector<QPoint>& points,
	      bool outline,
	      bool fill,
	      bool precise,
	      bool draw);

  ~ImageRegion();

  region::RegionId id();
  void paint(QPainter* painter,
	     const QStyleOptionGraphicsItem* option,
	     QWidget* widget = 0);

  QRectF boundingRect() const;
  QPainterPath shape() const;
  QString name() { return name_; }
  QRectF rect() { return rect_; }
  QString attr_id() { return attr_id_; }

public slots:
  void setOutline(bool outline);
  void setFill(bool fill);
  void setFillAlpha(int alpha);
  void setDraw(bool draw);
  void setPrecise(bool precise);
  void setDrawIfSameId(int id, bool draw);
  void select();
  void deselect();

private:
  region::RegionId id_;
  QString name_;
  QColor color_;
  QString attr_id_;
  QPainterPath shape_;
  QRectF rect_;
  bool outline_;
  bool precise_;
  bool fill_;
  bool draw_;
  bool selected_;
  int alpha_;
};

#include "image_region.hxx"

#endif // ! SCRIBO_DEMO_VIEWER_IMAGE_REGION_HH
