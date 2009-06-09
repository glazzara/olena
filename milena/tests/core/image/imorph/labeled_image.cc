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
#include <mln/core/image/imorph/labeled_image.hh>
#include <mln/core/routine/duplicate.hh>
#include <mln/make/image.hh>
#include <mln/make/box2d.hh>
#include <mln/value/label_8.hh>
#include <mln/accu/pair.hh>
#include <mln/accu/center.hh>
#include <mln/accu/bbox.hh>



static const unsigned bboxes_1[][4] = { { 1,1, 1,1 },
					{ 2,0, 2,1 },
					{ 0,0, 0,1 },
					{ 2,2, 2,2 } };


static const unsigned bboxes_2[][4] = { { 1,1, 1,1 },
					{ 2,2, 2,2 },
					{ 2,0, 2,1 },
					{ 0,0, 0,1 } };



namespace mln
{

  template <typename I>
  void test_image(const labeled_image<I>& lbl_i,
		  const unsigned bboxes[][4])
  {
    unsigned j = 0;
    for (unsigned i = 1; i < 5; ++i, ++j)
      mln_assertion(lbl_i.bbox(i) == make::box2d(bboxes[j][0], bboxes[j][1],
	    bboxes[j][2], bboxes[j][3]));
  }

} // end of namespace mln




int main()
{
  using namespace mln;
  using value::label_8;


  label_8 lbl_values[][3] = { { 3, 3, 0 },
			      { 0, 1, 0 },
			      { 2, 2, 4 } };

  typedef image2d<label_8> lbl_t;
  lbl_t lbl = make::image(lbl_values);

  labeled_image<lbl_t> lbl_i(lbl, 4);


  test_image(lbl_i, bboxes_1);

  fun::i2v::array<label_8> f(5);
  f(0) = 0;
  f(1) = 1;
  f(2) = 5;
  f(3) = 9;
  f(4) = 2;

  lbl_i.relabel(f);

  test_image(lbl_i, bboxes_2);
}
