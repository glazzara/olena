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

#include <QtGui>
#include "viewer.hh"

int main(int argc, char** argv)
{
  Viewer* viewer = Viewer::Instance(argc, argv);

  if (!viewer)
    return 0;

  return viewer->exec();
}
