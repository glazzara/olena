
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

#ifndef   	IMAGE_SCENE_HH_
# define   	IMAGE_SCENE_HH_

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

#endif	    /* !IMAGE_SCENE_HH_ */
