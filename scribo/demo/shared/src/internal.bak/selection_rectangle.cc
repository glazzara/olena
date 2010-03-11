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

#include <QtGui>
#include <shared/src/internal/selection_rectangle.hh>


namespace scribo
{

  namespace demo
  {

    namespace shared
    {

      namespace internal
      {

	selection_rectangle::selection_rectangle()
	{
	  QPen pen;
	  pen.setColor(QColor(Qt::red));
	  pen.setStyle(Qt::SolidLine);
	  pen.setWidth(1);


	  rects_.resize(9);
	  for (int i = 0; i < rects_.size(); ++i)
	  {
	    rects_[i] = new QGraphicsRectItem(this);
	    rects_[i]->setZValue(1);
	    rects_[i]->setPen(pen);
	    rects_[i]->show();
	    rects_[i]->setAcceptHoverEvents(true);
	  }

	  rects_[1]->setCursor(Qt::SizeFDiagCursor);
	  rects_[2]->setCursor(Qt::SizeVerCursor);
	  rects_[3]->setCursor(Qt::SizeBDiagCursor);
	  rects_[4]->setCursor(Qt::SizeHorCursor);
	  rects_[5]->setCursor(Qt::SizeHorCursor);
	  rects_[6]->setCursor(Qt::SizeBDiagCursor);
	  rects_[7]->setCursor(Qt::SizeVerCursor);
	  rects_[8]->setCursor(Qt::SizeFDiagCursor);

// 	  setHandlesChildEvents(true);
	  setCursor(Qt::SizeAllCursor);
	  setAcceptHoverEvents(true);
	}


	selection_rectangle::~selection_rectangle()
	{
	  foreach(QGraphicsRectItem *item, rects_)
	    delete item;
	}


	void selection_rectangle::paint(QPainter *painter,
					const QStyleOptionGraphicsItem *option,
					QWidget *widget)
	{
	  /*
	    --------------
	    |1|   2    |3|
	    |--========--|
	    | ||      || |
	    | ||      || |
	    |4||      ||5|
	    | ||      || |
	    | ||      || |
	    |--========--|
	    |6|   7    |8|
	    --------------
	   */

	  qreal border = 15;
	  qreal corner_width = qMin(qAbs(1/8.f * brect_.width()), border);
	  qreal corner_height = qMin(qAbs(1/8.f * brect_.height()), border);

	  painter->setBrush(QColor(255, 255, 255, 127));
	  painter->drawRect(brect_);

	  // Rectangle 1
	  QRectF rect1(brect_.topLeft().x(), brect_.topLeft().y(),
		       corner_width,
		       corner_height);
 	  rects_[1]->setRect(rect1);

	  // Rectangle 2
	  QRectF rect2(rect1.topRight().x(), rect1.topRight().y(),
		       brect_.width() - 2 * corner_width,
		       corner_height);
 	  rects_[2]->setRect(rect2);

 	  // Rectangle 3
	  QRectF rect3(brect_.topRight().x() - corner_width,
		       brect_.topRight().y(),
		       corner_width,
		       corner_height);
 	  rects_[3]->setRect(rect3);



	  // Rectangle 4
	  QRectF rect4(brect_.topLeft().x(),
		       brect_.topLeft().y() + corner_height,
		       corner_width,
		       brect_.height() - corner_height);
 	  rects_[4]->setRect(rect4);

	  // Rectangle 5
	  QRectF rect5(brect_.topRight().x() - corner_width,
		       brect_.topRight().y() + corner_height,
		       corner_width,
		       brect_.height() - corner_height);
 	  rects_[5]->setRect(rect5);



	  // Rectangle 6
	  QRectF rect6(brect_.bottomLeft().x(),
		       brect_.bottomLeft().y() - corner_height,
		       corner_width,
		       corner_height);
 	  rects_[6]->setRect(rect6);

	  // Rectangle 7
	  QRectF rect7(rect6.topRight().x(), rect6.topRight().y(),
		       brect_.width() - 2 * corner_width,
		       corner_height);
 	  rects_[7]->setRect(rect7);

 	  // Rectangle 8
	  QRectF rect8(rect7.topRight().x(), rect7.topRight().y(),
		       corner_width,
		       corner_height);
 	  rects_[8]->setRect(rect8);


	  painter->setPen(pen());
 	  painter->drawRect(brect_);
	}


	QRectF selection_rectangle::boundingRect() const
	{
	  qreal penWidth = pen().width();
	  return QRectF(brect_.x() - penWidth / 2, brect_.y() - penWidth / 2,
			brect_.width() + penWidth / 2,
			brect_.height() + penWidth / 2);
	}


	void selection_rectangle::setRect(qreal x, qreal y,
					  qreal width, qreal height)
	{
	  prepareGeometryChange();
	  brect_ = QRectF(x, y, width, height).normalized();
	}


	void selection_rectangle::setRect(const QRectF & rectangle)
	{
	  prepareGeometryChange();
	  brect_ = rectangle.normalized();
	}


	void selection_rectangle::update_size(QGraphicsItem *item,
					      const QPointF& p)
	{
	  /*
	    --------------
	    |1|   2    |3|
	    |--========--|
	    | ||      || |
	    | ||      || |
	    |4||      ||5|
	    | ||      || |
	    | ||      || |
	    |--========--|
	    |6|   7    |8|
	    --------------
	   */
	  QPointF p_ = p;

	  prepareGeometryChange();
	  if (rects_[1] == item)
	    brect_.setTopLeft(p_);
	  else if (rects_[2] == item)
	    brect_.setTop(p_.y());
	  else if (rects_[3] == item)
	    brect_.setTopRight(p_);
	  else if (rects_[4] == item)
	    brect_.setLeft(p_.x());
	  else if (rects_[5] == item)
	    brect_.setRight(p_.x());
	  else if (rects_[6] == item)
	    brect_.setBottomLeft(p_);
	  else if (rects_[7] == item)
	    brect_.setBottom(p_.y());
	  else if (rects_[8] == item)
	    brect_.setBottomRight(p_);
	  else
	    qDebug() << "selection_rectangle::update_size - invalid item";

	  brect_ = brect_.normalized();
	}


      } // end of namespace mln::demo::shared::internal

    } // end of namespace mln::demo::shared

  } // end of namespace mln::demo

} // end of namespace mln
