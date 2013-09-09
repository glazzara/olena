// Copyright (C) 2010, 2011, 2013 EPITA Research and Development Laboratory
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
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

/// \file
///
/// Test of scribo::toolchain::nepomuk::text_extraction

#include <QtGui/QImage>
#include <QtCore>
#include <scribo/toolchain/nepomuk/text_extraction.hh>

#include "tests/data.hh"

int main()
{
  QImage ima(SCRIBO_IMG_DIR "/wildly.pbm");
  QSet<QString> words = scribo::toolchain::nepomuk::text_extraction(ima);

  mln_assertion(words.size() == 1);
  QString word = words.toList().at(0).toLower();
  mln_assertion(word == "wildly");

  return 0;
}
