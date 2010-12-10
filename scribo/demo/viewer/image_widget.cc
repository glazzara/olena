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

  view_->setDragMode(QGraphicsView::ScrollHandDrag);
  view_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  view_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  view_->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

  connect(view_, SIGNAL(scaleUpdated(qreal)),
	  this, SIGNAL(scaleUpdated(qreal)));

  setLayout(layout);
}

void
ImageWidget::update()
{
  view_->fitInView(view_->sceneRect(), Qt::KeepAspectRatio);
  view_->scaleUpdate();
}

ImageWidget::~ImageWidget()
{
}
