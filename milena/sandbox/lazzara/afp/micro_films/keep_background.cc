// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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
/// Keep the background from a binary image.

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/pw/all.hh>
#include <mln/core/image/dmorph/image_if.hh>

#include <mln/data/fill.hh>
#include <mln/io/pbm/all.hh>
#include <mln/labeling/blobs.hh>

#include <mln/logical/not.hh>

#include <mln/value/label_16.hh>

#include <tools/usage.hh>


const char *args_desc[][2] =
{
  { "input.pbm", "A binary image. Objects are set to 'true'." },
  { 0, 0 }
};



int main(int argc, char *argv[])
{
  using namespace mln;

  if (argc != 3)
    return tools::usage(argv,
 		 "Extract the background from a binary image.",
		 "input.pbm output.pbm",
		 args_desc, "A binary image. Background is set to 'false'.");

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  logical::not_inplace(input);

  typedef value::label_16 V;
  V nlabels;
  image2d<V> lbl = labeling::blobs(input, c8(), nlabels);

  image2d<bool> output;
  initialize(output, input);

  V bg_lbl = lbl(literal::origin);

  data::fill(output, false);
  data::fill((output | (pw::value(lbl) == pw::cst(bg_lbl))).rw(), true);

  io::pbm::save(output, argv[2]);
}

