// Copyright (C) 2009 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#include <mln/essential/2d.hh>
#include <mln/util/couple.hh>

#include <scribo/table/repair_vertical_lines.hh>
#include <scribo/table/repair_horizontal_lines.hh>
#include <scribo/table/extract_lines_with_rank.hh>
#include <scribo/debug/save_table_image.hh>

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

  typedef util::couple<util::array<box2d>,
		       util::array<box2d> > tblboxes_t;

  tblboxes_t lineboxes;
  lineboxes.first().append(make::box2d(0,0, 1,1)); // Dummy value for component 0.
  lineboxes.first().append(make::box2d(0,28, 27, 32));
  lineboxes.first().append(make::box2d(31,28, 59,32));
  lineboxes.second().append(make::box2d(0,0, 1,1)); // Dummy value for component 0.
  lineboxes.second().append(make::box2d(27,0, 31, 26));
  lineboxes.second().append(make::box2d(27,34, 31,59));


//  tblboxes_t lineboxes = scribo::table::extract_lines_with_rank(input, c8(), value::label_16(), win::vline2d(11), win::hline2d(11), 2, 2);
//
//  std::cout << lineboxes.first() << std::endl;
//  std::cout << lineboxes.second() << std::endl;

  scribo::table::repair_vertical_lines(input, lineboxes, 31);
  scribo::table::repair_horizontal_lines(input, lineboxes, 31);

  mln_assertion(lineboxes.first().nelements() == 2);
  mln_assertion(lineboxes.second().nelements() == 2);
  mln_assertion(lineboxes.first()[1] == make::box2d(0,28, 59,32));
  mln_assertion(lineboxes.second()[1] == make::box2d(27,0, 31,59));
}
