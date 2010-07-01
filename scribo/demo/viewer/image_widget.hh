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

#ifndef   	IMAGE_WIDGET_HH_
# define   	IMAGE_WIDGET_HH_

# include <QtGui>

class ImageView;

class ImageWidget
  : public QWidget
{
  Q_OBJECT

public:
  ImageWidget(QGraphicsScene* scene);
  ~ImageWidget();

public slots:
  void update();

signals:
  void scaleUpdated(qreal scale);

private:
  ImageView* view_;
};

#endif	    /* !IMAGE_WIDGET_HH_ */
