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

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/box2d.hh>
#include <mln/make/box2d.hh>
#include <mln/util/array.hh>
#include <mln/util/couple.hh>
#include <mln/io/pbm/load.hh>
#include <mln/value/label_8.hh>
#include <mln/win/vline2d.hh>
#include <mln/win/hline2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/literal/colors.hh>

#include <mln/io/pgm/save.hh>

#include <scribo/primitive/extract/lines_h_discontinued.hh>
#include <scribo/primitive/extract/lines_v_discontinued.hh>

#include <scribo/tests/data.hh>


int main(int argc, char *argv[])
{
  using namespace mln;

  std::string img;
  if (argc < 2)
    img = SCRIBO_IMG_DIR "/table_to_be_repaired.pbm";
  else
    img = argv[1];

  image2d<bool> input;
  io::pbm::load(input, img.c_str());

  typedef util::array<box2d> lineboxes_t;

  lineboxes_t
    vboxes,
    hboxes;
  vboxes.append(make::box2d(0,0, 59,59));
  vboxes.append(make::box2d(0,28, 27, 32));
  vboxes.append(make::box2d(31,28, 59,32));
  hboxes.append(make::box2d(0,0, 59,59));
  hboxes.append(make::box2d(27,0, 31, 26));
  hboxes.append(make::box2d(27,34, 31,59));

  value::label_8 nbboxes;

  lineboxes_t hboxes_, vboxes_;

  typedef object_image(image2d<value::label_8>) lbl_t;
  lbl_t
    hlbl = scribo::primitive::extract::lines_h_discontinued(input, c8(),
							    nbboxes, 11, 2);
  lbl_t
    vlbl = scribo::primitive::extract::lines_v_discontinued(input, c8(),
							    nbboxes, 11, 2);
  mln_assertion(hlbl.nlabels() == 2);
  mln_assertion(vlbl.nlabels() == 2);
  // Background values are meaningless, we skip them.
  for (unsigned i = 1; i < 3; ++i)
  {
    mln_assertion(hlbl.bbox(i) == hboxes(i));
    mln_assertion(vlbl.bbox(i) == vboxes(i));
  }

}
