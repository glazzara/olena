// Copyright (C) 2010, 2013 EPITA Research and Development Laboratory (LRDE)
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

#include <libgen.h>
#include <iostream>
#include <QtCore/QDebug>

#include <scribo/debug/usage.hh>
#include <scribo/toolchain/nepomuk/text_extraction.hh>


const char *args_desc[][2] =
{
  { "input.*", "An image." },
  {0, 0}
};




int main(int argc, char* argv[])
{
  if (argc != 2)
    return scribo::debug::usage(argv,
				"Extract text in a document.\n\n"
				"Common usage: ./text_extraction input.*",
				"<image>",
				args_desc);


  QImage ima(argv[1]);
  QSet<QString> words = scribo::toolchain::nepomuk::text_extraction(ima);

  foreach(QString w, words)
    qDebug() << w;
}
