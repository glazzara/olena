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

#ifndef   	IMAGE_REGION_HXX_
# define   	IMAGE_REGION_HXX_

# include "image_region.hh"

inline
void
ImageRegion::setOutline(bool outline)
{
  outline_ = outline;
  scene()->invalidate();
  update();
}

inline
void
ImageRegion::setPrecise(bool precise)
{
  precise_ = precise;
  scene()->invalidate();
  update();
}

inline
void
ImageRegion::setFill(bool fill)
{
  fill_ = fill;
  update();
}

inline
void
ImageRegion::setDraw(bool draw)
{
  draw_ = draw;
  update();
}

inline
void
ImageRegion::setDrawIfSameId(int id, bool draw)
{
  if (id == id_)
    {
      draw_ = draw;
      setVisible(draw);
      if (scene())
	{
	  scene()->invalidate();
	  update();
	}
    }
}

inline
void
ImageRegion::setFillAlpha(int alpha)
{
  alpha_ = alpha;
  update();
}

inline
QRectF
ImageRegion::boundingRect() const
{
//   if (selected_)
//     return mapFromScene(scene()->sceneRect()).boundingRect();
  return rect_;
}

inline
QPainterPath
ImageRegion::shape() const
{
  return shape_;
}

#endif	    /* !IMAGE_REGION_HXX_ */
