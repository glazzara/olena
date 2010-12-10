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

#ifndef SCRIBO_DEMO_VIEWER_IMAGE_SCENE_HH
# define SCRIBO_DEMO_VIEWER_IMAGE_SCENE_HH

# include <QtGui>

class ImageRegion;

class ImageScene
  : public QGraphicsScene
{
  Q_OBJECT

public:
  ImageScene();
  ~ImageScene();

  void mousePressEvent(QGraphicsSceneMouseEvent* event);
  void clear();

signals:
  void selected(QString, QString);
  void deselected();

private:
  ImageRegion* selected_;
  QVector<ImageRegion*> region_vector_;
};

#endif // ! SCRIBO_DEMO_VIEWER_IMAGE_SCENE_HH
