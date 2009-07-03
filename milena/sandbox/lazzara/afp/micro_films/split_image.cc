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
/// Split an image into several smaller image based on the objects.

#include <mln/core/image/image2d.hh>
#include <mln/core/image/imorph/labeled_image.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/data/fill.hh>
#include <mln/debug/filename.hh>
#include <mln/io/pbm/all.hh>
#include <mln/io/pgm/all.hh>
#include <mln/labeling/blobs.hh>
#include <mln/value/int_u8.hh>

#include <tools/usage.hh>


const char *args_desc[][2] =
{
  { "input.pgm", "A gray level image." },
  { "mask.pbm",  "A binary image. Objects are set to 'true'. Will be used to split input.pbm into several images." },
  { 0, 0 }
};



int main(int argc, char *argv[])
{
  using namespace mln;

  if (argc != 3)
    return tools::usage(argv,
 		 "Extract an image for each object in the input image.",
		 "input.pgm mask.pbm",
		 args_desc, "Gray level images extracted from the input.");

  image2d<value::int_u8> input;
  io::pgm::load(input, argv[1]);

  image2d<bool> mask;
  io::pbm::load(mask, argv[2]);

  value::int_u8 nlabels;
  typedef image2d<value::int_u8> lbl_t;
  lbl_t lbl_ = labeling::blobs(mask, c8(), nlabels);
  labeled_image<lbl_t> lbl(lbl_, nlabels);

  debug::internal::filename_prefix = "split";

  for (unsigned i = 1; i < lbl.nlabels(); ++i)
    io::pgm::save(input | lbl.bbox(i), debug::filename("object.pgm"));

}
