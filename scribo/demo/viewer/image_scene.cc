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

#include "image_scene.hh"
#include "image_region.hh"

ImageScene::ImageScene()
  : selected_(0)
{
}

void
ImageScene::clear()
{
  selected_ = 0;
  QGraphicsScene::clear();
}

void
ImageScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
  QGraphicsScene::mousePressEvent(event);
  QList<QGraphicsItem *> items_list = items(event->scenePos()); // includes both ImageRegions and the picture.

  if (items_list.isEmpty())
    {
      if (selected_)
	{
	  selected_->deselect();
	  emit deselected();
	  selected_ = 0;
	}
    }

  // Selection is under the mouse click (at event->pos()).
  bool selection_is_clicked = items_list.contains(selected_);

  foreach(QGraphicsItem* elt, items_list)
    {
      ImageRegion* item = dynamic_cast<ImageRegion*>(elt);
      if (item)
	{
	  if (item != selected_)
	    {
	      if (selected_)
		{
		  if ( (item->boundingRect().intersects(selected_->boundingRect())))
		    {
		      int item_area = item->boundingRect().size().height() * item->boundingRect().size().width();
		      int selected_area = selected_->boundingRect().size().height() * selected_->boundingRect().size().width();

		      if (selected_area < item_area && selection_is_clicked)
			return;
		      else
			{
			  selected_->deselect();
			  emit deselected();
			  selected_ = 0;
			}
		    }
		  else
		    {
		      selected_->deselect();
		      emit deselected();
		      selected_ = 0;
		    }
		}
	      selected_ = item;
	      item->select();
	      emit selected(item->attr_id(), item->name());
	      return;
	    }
	}
      else
	{
	  if ( (selected_)
	       && (items_list.size() == 1) )// no ImageRegion, only the picture
	    {
	      selected_->deselect();
	      emit deselected();
	      selected_ = 0;
	    }
	}
    }
}

ImageScene::~ImageScene()
{
}
