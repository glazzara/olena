// Copyright (C) 2010, 2011 EPITA Research and Development Laboratory
// (LRDE)
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

#include <QtGui>
#include <iostream>

# ifdef HAVE_TESSERACT_2
#include <clocale>
# endif // ! HAVE_TESSERACT_2

#undef MLN_WO_GLOBAL_VARS
#include "viewer.hh"
#include <mln/labeling/colorize.hh>
#include <mln/math/pi.hh>
#include <mln/io/magick/load.hh>
#include <scribo/make/debug_filename.hh>
#include <mln/debug/filename.hh>
#include <mln/literal/colors.hh>

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

  Magick::InitializeMagick(*argv);

  Viewer* viewer = Viewer::Instance(argc, argv);

  if (!viewer)
    return -1;


# ifdef HAVE_TESSERACT_2
  // Tesseract 2.x is known to have issues while reading training data
  // depending on the current locales in use. Training data files use
  // float data and the decimal separator can be either '.' or ','
  // causing errors.
  // Setting locale to "C" fix that issue.
  setlocale(LC_ALL, "C");
# endif // ! HAVE_TESSERACT_2

  return viewer->exec();
}
