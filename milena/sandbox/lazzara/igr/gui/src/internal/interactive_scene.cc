// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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
	: QGraphicsScene(parent)
      {
	line_.setZValue(1);

	QPen pen;
	pen.setColor(QColor(Qt::red));
	pen.setStyle(Qt::SolidLine);
	pen.setWidth(1);

	line_.setPen(pen);
      }


      void interactiveScene::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
      {
	line_.setLine(QLineF(p_start_, e->scenePos()));
	mouse_moved_ = true;

	e->accept();
      }

      void interactiveScene::mousePressEvent(QGraphicsSceneMouseEvent *e)
      {
	p_start_ = e->scenePos();
	mouse_moved_ = false;

	line_.setLine(QLineF(e->scenePos(), e->scenePos()));
	addItem(&line_);

	e->accept();
      }

      void interactiveScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
      {
	if (line_.scene() == this)
	  removeItem(&line_);

 	if (mouse_moved_)
	  emit mouse_draw_line(p_start_, e->scenePos());
 	else
 	  emit mouse_click(e->scenePos());

	e->ignore();
      }


    } // end of namespace mln::demo::internal

  } // end of namespace mln::demo

} // end of namespace mln
