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

#ifndef SCRIBO_DEMO_SHARED_SRC_INTERNAL_SELECTION_RECTANGLE_HH
# define SCRIBO_DEMO_SHARED_SRC_INTERNAL_SELECTION_RECTANGLE_HH

# include <QtCore>
# include <QtGui>

namespace scribo
{

  namespace demo
  {

    namespace shared
    {

      namespace internal
      {

	class selection_rectangle : public QAbstractGraphicsShapeItem
	{

	public:
	  selection_rectangle();
	  ~selection_rectangle();

	  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
		     QWidget *widget);

	  QRectF boundingRect() const;

	  void setRect(qreal x, qreal y, qreal width, qreal height);
	  void setRect(const QRectF & rectangle);

	  void update_size(QGraphicsItem *item, const QPointF& p);

	private: // members


	private: // attributes
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
	  QVector<QGraphicsRectItem *> rects_;
	  QRectF brect_;

	};

      } // end of namespace mln::demo::shared::internal

    } // end of namespace mln::demo::shared

  } // end of namespace mln::demo

} // end of namespace mln

#endif // !SCRIBO_DEMO_SHARED_SRC_INTERNAL_SELECTION_RECTANGLE_HH
