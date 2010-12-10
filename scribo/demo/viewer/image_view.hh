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

#ifndef SCRIBO_DEMO_VIEWER_IMAGE_VIEW_HH
# define SCRIBO_DEMO_VIEWER_IMAGE_VIEW_HH

# include <QtGui>

class ImageView
  : public QGraphicsView
{
  Q_OBJECT

public:
  ImageView(QGraphicsScene* scene);
  ~ImageView();

  void wheelEvent(QWheelEvent* event);
  void keyPressEvent(QKeyEvent *event);

  // Call after changing the scale.
  void scaleUpdate();

signals:
  // Scale is approximate.
  void scaleUpdated(qreal scale);
};

#endif // ! SCRIBO_DEMO_VIEWER_IMAGE_VIEW_HH
