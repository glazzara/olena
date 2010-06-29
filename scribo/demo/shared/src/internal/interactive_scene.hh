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

#ifndef SCRIBO_DEMO_SHARED_SRC_INTERNAL_INTERACTIVE_SCENE_HH
# define SCRIBO_DEMO_SHARED_SRC_INTERNAL_INTERACTIVE_SCENE_HH

# include <QtGui/QGraphicsScene>
# include <QtGui/QGraphicsSceneMouseEvent>
# include <QtGui/QGraphicsLineItem>
# include <QtCore/QPoint>
# include <QtCore/QDebug>

# include <shared/src/internal/selection_rectangle.hh>

namespace scribo
{

  namespace demo
  {

    namespace shared
    {

      namespace internal
      {

	class interactiveScene : public QGraphicsScene
	{
	  Q_OBJECT;

	  typedef QGraphicsScene super_;

	  public:

	  interactiveScene(QObject * parent = 0);

	  void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
	  void mousePressEvent(QGraphicsSceneMouseEvent *e);
	  void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);

	signals:
	  void mouse_click(const QPointF& p);
	  void mouse_moved(const QPointF& p);
	  void mouse_pressed(const QPointF& p);
	  void mouse_released(const QPointF& p);

	private:
	  QPointF pressed_point_;

	  bool mouse_moved_;
	};


      } // end of namespace mln::demo::shared::internal

    } // end of namespace mln::demo::shared

  } // end of namespace mln::demo

} // end of namespace mln

#endif // ! SCRIBO_DEMO_SHARED_SRC_INTERNAL_INTERACTIVE_SCENE_HH
