// Copyright (C) 2009, 2010 EPITA Research and Development Laboratory
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
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

# include <src/internal/interactive_scene.hh>

namespace mln
{

  namespace demo
  {

    namespace internal
    {

      interactiveScene::interactiveScene(QObject * parent)
	: QGraphicsScene(parent), selectionTracking_(false)
      {
// 	line_.setZValue(1);
 	rect_.setZValue(1);

	QPen pen;
	pen.setColor(QColor(Qt::red));
	pen.setStyle(Qt::SolidLine);
	pen.setWidth(1);

// 	line_.setPen(pen);
 	rect_.setPen(pen);
      }


      void interactiveScene::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
      {
	if (selectionTracking_ && mouse_pressed_)
	  rect_.setRect(QRectF(p_start_, e->scenePos()));
// 	  line_.setLine(QLineF(p_start_, e->scenePos()));

	mouse_moved_ = true;

	if (mouse_pressed_)
	  emit mouse_pressed(e->scenePos());

	emit mouse_moved(e->scenePos());

	e->accept();
      }

      void interactiveScene::mousePressEvent(QGraphicsSceneMouseEvent *e)
      {
	p_start_ = e->scenePos();
	mouse_moved_ = false;
	mouse_pressed_ = true;

	if (selectionTracking_)
	{
// 	  line_.setLine(QLineF(e->scenePos(), e->scenePos()));
// 	  addItem(&line_);
	  rect_.setRect(QRectF(e->scenePos(), e->scenePos()));
	  if (rect_.scene() != this)
	    addItem(&rect_);
	}

	emit mouse_pressed(e->scenePos());

	e->accept();
      }

      void interactiveScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
      {
// 	if (selectionTracking_ && line_.scene() == this)
// 	  removeItem(&line_);

	mouse_pressed_ = false;

 	if (mouse_moved_ && selectionTracking_)
	  emit new_selection(rect_.rect());
 	else
	{
	  if (rect_.scene() == this)
	  {
	    rect_.setRect(sceneRect());
	    removeItem(&rect_);
	    emit selection_cleared();
	  }

	  emit mouse_click(e->scenePos());
	}

	emit mouse_released(e->scenePos());

	e->ignore();
      }


      void interactiveScene::setSelectionTracking(bool b)
      {
	selectionTracking_ = b;
      }

      bool interactiveScene::has_selection() const
      {
	return rect_.scene() == this;
      }

      QRectF interactiveScene::selection() const
      {
	return rect_.rect();
      }

      void interactiveScene::clear_selection()
      {
 	if (rect_.scene() == this)
	  removeItem(&rect_);
      }


    } // end of namespace mln::demo::internal

  } // end of namespace mln::demo

} // end of namespace mln
