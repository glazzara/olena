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
//
// Based on the code located here:
// http://th30z.netsons.org/2010/01/qt4-image-crop-item/

#ifndef SCRIBO_DEMO_SHARED_SRC_CROP_ITEM_HH
# define SCRIBO_DEMO_SHARED_SRC_CROP_ITEM_HH

#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QApplication>
#include <QPainter>
#include <QtGui>

#define CROP_BORDER_LINE        10
#define CROP_GRID_SIZE           3

namespace scribo
{

  namespace demo
  {

    namespace shared
    {

      class crop_item : public QObject, public QGraphicsItem
      {
	Q_OBJECT;

      public:
	crop_item(QGraphicsItem *parent);
	virtual ~crop_item();

	QRectF cropRect() const;

	QRectF boundingRect() const;
	void paint (QPainter *painter,
		    const QStyleOptionGraphicsItem *option,
		    QWidget *widget = 0);

	void reset();

      protected:
	void mousePressEvent (QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent (QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);

//	void updateCursor(QGraphicsSceneMouseEvent *event);

      signals:
	void ready_for_crop();

      private:
	enum CropItemResize
	{
	  CropItemResizeNone,

	  CropItemResizeTopLeft,
	  CropItemResizeTopRight,
	  CropItemResizeBottomLeft,
	  CropItemResizeBottomRight,

	  CropItemResizeLeft,
	  CropItemResizeRight,
	  CropItemResizeTop,
	  CropItemResizeBottom
	};

      private:
	CropItemResize cropResize_;
	QRectF cropRect_;
	bool mousePress_;
      };


    }  // end of namespace scribo::demo::shared

  } // end of namespace scribo::demo

} // end of namespace scribo


#endif // ! SCRIBO_DEMO_SHARED_SRC_CROP_ITEM_HH
