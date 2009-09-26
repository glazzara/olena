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

#include "image_widget.hh"
#include "image_view.hh"

ImageWidget::ImageWidget(QGraphicsScene* scene)
  : view_ (new ImageView(scene))
{
  QLabel* title = new QLabel(tr("Layout"));
  title->setAlignment(Qt::AlignHCenter);

  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(title);
  layout->addWidget(view_);

  view_->setDragMode (QGraphicsView::ScrollHandDrag);
  view_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  view_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  view_->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

  setLayout(layout);
}

void
ImageWidget::update()
{
  view_->fitInView(view_->sceneRect(), Qt::KeepAspectRatio);
}

ImageWidget::~ImageWidget()
{
}
