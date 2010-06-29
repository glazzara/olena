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

# include <shared/src/internal/interactive_scene.hh>

namespace scribo
{

  namespace demo
  {

    namespace shared
    {

      namespace internal
      {


	struct test : public QGraphicsItem
	{
	  test()
	  {
	    setZValue(0);
	    setCursor(Qt::OpenHandCursor);

	  }

	  void mousePressEvent(QGraphicsSceneMouseEvent *event)
	  {
	    setCursor(Qt::ClosedHandCursor);
	  }

	  void mouseMoveEvent(QGraphicsSceneMouseEvent *event)
	  {
	    setCursor(Qt::OpenHandCursor);
	  }

	  void mouseReleaseEvent(QGraphicsSceneMouseEvent *)
	  {
	    setCursor(Qt::OpenHandCursor);
	  }

	  void paint(QPainter *painter,
		     const QStyleOptionGraphicsItem *option,
		     QWidget *widget)
	  {
	    painter->setPen(Qt::NoPen);
	    painter->setBrush(Qt::darkGray);
	    painter->drawEllipse(-12, -12, 30, 30);
	    painter->setPen(QPen(Qt::black, 1));
	    painter->setBrush(QBrush(Qt::red));
	    painter->drawEllipse(-15, -15, 30, 30);

	    painter->setBrush(QColor(0, 0, 0, 127));
	    painter->drawRect(scene()->sceneRect());

	  }

	  QRectF boundingRect() const
	  {
	    return QRectF(-15.5, -15.5, 34, 34);
	  }

	};



	interactiveScene::interactiveScene(QObject * parent)
	  : QGraphicsScene(parent), mode_(Drawing),
	    mouse_moved_(false), mouse_pressed_(false),
	    selectionTracking_(false)
	{
	  rect_.setZValue(1);

	  QPen pen;
	  pen.setColor(QColor(Qt::black));
	  pen.setStyle(Qt::SolidLine);
	  pen.setWidth(1);

	  rect_.setPen(pen);

	  setSceneRect(QRectF(-150, -150, 300, 300));

	  addItem(new test());
	}


	void interactiveScene::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
	{
	  if (mouse_pressed_)
	  {
	    // If selection is enabled.
	    if (selectionTracking_)
	    {
	      switch(mode_)
	      {
		// Moving an existing selection.
		case Moving:
		{
		  QPointF p = e->scenePos() - e->lastScenePos();
		  rect_.translate(p.x(), p.y());
		  e->accept();
		  break;
		}

		// A new selection must be extended.
		case Drawing:
		{
		  rect_.setRect(QRectF(e->buttonDownScenePos(Qt::LeftButton),
				       e->scenePos()));
		  e->accept();
		  break;
		}


		// Extending an existing selection
		case Extending:
		{
		  rect_.update_size(rect_corner_, e->scenePos());
		  break;
		}

		default:
		  qDebug() << "interactiveScene::mouseMoveEvent - Invalid mode";
	    }

	    emit mouse_pressed(e->scenePos());
	    mouse_moved_ = true;
	    }
	  }
	  else
	    super_::mouseMoveEvent(e);

	  emit mouse_moved(e->scenePos());
	}

	void interactiveScene::mousePressEvent(QGraphicsSceneMouseEvent *e)
	{
	  mouse_moved_ = false;
	  mouse_pressed_ = true;
	  pressed_point_ = e->scenePos();

	  emit mouse_pressed(e->scenePos());

	  // If selection is enabled.
	  if (selectionTracking_)
	  {
	    // About to move an existing rectangle.
	    QGraphicsItem *item = itemAt(e->scenePos());
	    if (item == &rect_)
	    {
	      mode_ = Moving;
	      super_::mousePressEvent(e);
	    }
	    else if (item != 0)
	    {
	      mode_ = Extending;
	      rect_corner_ = item;
	      super_::mousePressEvent(e);
	    }
	    // Draw a new rectangle
	    else
	    {
	      mode_ = Drawing;
	      rect_.resetTransform();
	      rect_.setRect(QRectF(e->scenePos(), e->scenePos()));
	      if (rect_.scene() != this)
		addItem(&rect_);
	      e->accept();
	    }
	  }
	}

	void interactiveScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
	{
	  mouse_pressed_ = false;

	  switch(mode_)
	  {

	    case Drawing:
	    {
	      if (selectionTracking_ && mouse_moved_)
		emit new_selection(rect_.boundingRect());
	      else
	      {
		// A mouse click is performed outside the selection
		// rectangle.
		// Clear selection if exists.
		if (mode_ != Moving && rect_.scene() == this)
		{
		  rect_.setRect(sceneRect());
		  removeItem(&rect_);
		  emit selection_cleared();
		}
	      }
	      break;
	    }

	    case Moving:
	    {
	      if (mouse_moved_)
	      {
		QPointF p = e->scenePos() - pressed_point_;


		// FIXME: the bounding rectangle size is increasing by
		// 0.5 pixels for an unknown reason...
		rect_.setRect(rect_.boundingRect().translated(p.x(), p.y()));
		rect_.resetTransform();
	      }
	      break;
	    }


	    case Extending:
	    {
	      break;
	    }

	    default:
	      qDebug() << "interactiveScene::mouseReleaseEvent - invalid mode";

	  }


	  if (!mouse_moved_)
	    emit mouse_click(e->scenePos());

	  emit mouse_released(e->scenePos());

	  e->accept();
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
	  return rect_.boundingRect();
	}

	void interactiveScene::clear_selection()
	{
	  if (rect_.scene() == this)
	    removeItem(&rect_);
	}


      } // end of namespace mln::demo::shared::internal

    } // end of namespace mln::demo::shared

  } // end of namespace mln::demo

} // end of namespace mln
