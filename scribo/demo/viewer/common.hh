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

#ifndef   	COMMON_HH_
# define   	COMMON_HH_

namespace region
{
  typedef QVector<QPair<QString, QColor> > KeyMap;

  enum RegionId
    {
      Text = 0,
      Paragraph,
      Line,
      Image, 
      Noise,
      Separator,
      Table,
      LineDrawing,
      Graphic,
      Chart,
      Maths
    };
}

#endif	    /* !COMMON_HH_ */
