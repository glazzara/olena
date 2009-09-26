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

#ifndef   	VIEWER_HXX_
# define   	VIEWER_HXX_

# include "viewer.hh"

inline Viewer* Viewer::Instance(int &argc, char** argv)
{
  static Viewer viewer(argc, argv);

  return &viewer;
}

inline Viewer::~Viewer()
{
}

#endif	    /* !VIEWER_HXX_ */
