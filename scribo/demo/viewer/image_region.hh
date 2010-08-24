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

#ifndef   	IMAGE_REGION_HH_
# define   	IMAGE_REGION_HH_

# include <QtGui>
# include "common.hh"

class ImageRegion
  : public QObject, public QGraphicsItem
{
  Q_OBJECT

public:
  ImageRegion(region::RegionId id,
	      QString name,
	      QColor color,
	      QModelIndex index,
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

  const QModelIndex& index() const;
  QRectF boundingRect() const;
  QPainterPath shape() const;
  QString name() { return name_; }

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
  QModelIndex index_;
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

#endif	    /* !IMAGE_REGION_HH_ */
