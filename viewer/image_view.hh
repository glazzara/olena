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

#ifndef   	IMAGE_VIEW_HH_
# define   	IMAGE_VIEW_HH_

# include <QtGui>

class ImageView
  : public QGraphicsView
{
  Q_OBJECT

public:
  ImageView(QGraphicsScene* scene);
  ~ImageView();

  void wheelEvent(QWheelEvent* event);
};

#endif	    /* !IMAGE_VIEW_HH_ */
