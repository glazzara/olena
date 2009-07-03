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
///

#include <mln/core/image/image2d.hh>
#include <mln/core/image/imorph/labeled_image.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/var.hh>

#include <mln/data/fill.hh>
#include <mln/debug/filename.hh>
#include <mln/io/pbm/all.hh>
#include <mln/io/pgm/all.hh>
#include <mln/labeling/blobs.hh>
#include <mln/labeling/background.hh>
#include <mln/labeling/n_max.hh>

#include <mln/value/label_16.hh>

#include <mln/geom/rotate.hh>
#include <mln/data/fill.hh>
#include <mln/logical/not.hh>

#include <tools/usage.hh>
#include <scribo/preprocessing/unskew.hh>
#include <scribo/filter/small_objects.hh>

#include <mln/data/compare.hh>


const char *args_desc[][2] =
{
  { "input.pgm", "A gray level image." },
  { "input.pbm", "A binary image." },
  { "mask.pbm",  "A binary image. Objects are set to 'true'. Will be used to split input.pbm into several images." },
  { "rot",       "Initial rotation in degrees." },
  { 0, 0 }
};



int main(int argc, char *argv[])
{
  using namespace mln;

  if (argc != 5)
    return tools::usage(argv,
 		 "Extract an image for each object in the input image.",
		 "input.pgm input.pbm mask.pbm rot",
		 args_desc, "Gray level images extracted from the input.");

  image2d<value::int_u8> input;
  io::pgm::load(input, argv[1]);

  image2d<bool> input_bw;
  io::pbm::load(input_bw, argv[2]);

  image2d<bool> mask;
  io::pbm::load(mask, argv[3]);

  value::label_16 nlabels;
  typedef image2d<value::label_16> lbl_t;
  lbl_t lbl_ = labeling::blobs(mask, c8(), nlabels);
  labeled_image<lbl_t> lbl(lbl_, nlabels);

  debug::internal::filename_prefix = "mf";


  value::label_16 nbglabels;
  lbl_t lbl_bg = labeling::background(input_bw, c8(), nbglabels);

  // Should be always true...
  value::label_16 bg = lbl_bg(point2d(0,0));
  // ... but not as robust as the line below.
//  value::label_16 bg = labeling::n_max(lbl_bg, nbglabels, 2)[1];

  logical::not_inplace(input_bw);

  for (unsigned i = 1; i <= lbl.nlabels(); ++i)
  {
    io::pbm::save(input_bw | lbl.bbox(i), debug::filename(".pbm", i));
    mln_VAR(tmp_bw, input_bw | lbl.bbox(i));
    data::fill((tmp_bw | (pw::value(lbl_bg) == pw::cst(bg))).rw(), false);

    mln_VAR(cleaned,
	    scribo::preprocessing::unskew(geom::rotate(tmp_bw, atoi(argv[4]))));

    double angle = cleaned.second();

//    io::pbm::save(cleaned.first(), debug::filename("object.pbm"));
    mln_VAR(cell, input | lbl.bbox(i));
    data::fill((cell | (pw::value(lbl_bg) == pw::cst(bg))).rw(),
	       mln_max(value::int_u8));
    io::pgm::save(geom::rotate(cell, angle + atoi(argv[4]),
			       mln_max(value::int_u8)),
		  debug::filename(".pgm", i));
  }

}
