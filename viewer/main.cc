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
#include <iostream>

#include "viewer.hh"

int main(int argc, char** argv)
{
  if (argc > 2 ||
      (argc == 2 &&
       (QString(argv[1]) == "--help" || QString(argv[1]) == "-h")))
  {
    std::cout << "Usage:" << std::endl
	      << argv[0] << " <image dir>" << std::endl;
    return 0;
  }

  Viewer* viewer = Viewer::Instance(argc, argv);

  if (!viewer)
    return -1;

  return viewer->exec();
}
