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
  ImageRegion* item = dynamic_cast<ImageRegion*>(itemAt(event->pos()));
  if (item)
  {
    if (item != selected_)
    {
      if (selected_)
      {
	selected_->deselect();
	emit deselected(selected_->index());
      }
      selected_ = item;
      item->select();
      emit selected(item->index());
    }
  }
  else
  {
    if (selected_)
    {
      selected_->deselect();
      emit deselected(selected_->index());
      selected_ = 0;
    }
  }
}

ImageScene::~ImageScene()
{
}
