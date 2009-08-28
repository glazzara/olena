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
#include <mln/accu/shape/bbox.hh>


# include <mln/debug/println.hh>

static const unsigned bboxes_1[][9] = { { 1,1, 1,1 }, // 1
					{ 2,2, 2,2 }, // 2
					{ 0,0, 0,0 },
					{ 0,0, 0,0 },
					{ 2,0, 2,1 }, // 5
					{ 0,0, 0,0 },
					{ 0,0, 0,0 },
					{ 0,0, 0,0 },
					{ 0,0, 0,1 } };


static const unsigned bboxes_2[][4] = { { 1,1, 1,1 },
					{ 2,2, 2,2 },
					{ 2,0, 2,1 },
					{ 0,0, 0,1 } };



namespace mln
{

  template <typename I, unsigned n>
  void test_image(const labeled_image<I>& lbl_i,
		  const unsigned bboxes[][n])
  {
    unsigned
      j = 0,
      k = 0;
    for (unsigned i = 1; i <= lbl_i.nlabels(); ++i, ++j)
      if (lbl_i.bbox(i).is_valid())
      {
	mln_assertion(lbl_i.bbox(i) == make::box2d(bboxes[j][0], bboxes[j][1],
						   bboxes[j][2], bboxes[j][3]));
	++k;
      }

    mln_assertion(k == 4);
  }

} // end of namespace mln




int main()
{
  using namespace mln;
  using value::label_8;


  label_8 lbl_values[][3] = { { 9, 9, 0 },
			      { 0, 1, 0 },
			      { 5, 5, 2 } };

  typedef image2d<label_8> lbl_t;
  lbl_t lbl = make::image(lbl_values);

  labeled_image<lbl_t> lbl_i(lbl, 9);
  mln_assertion(lbl_i.nlabels() == 9);
  test_image(lbl_i, bboxes_1);

  fun::i2v::array<label_8> f(10, 0);
  f(0) = 0;
  f(1) = 1;
  f(5) = 2;
  f(9) = 5;
  f(2) = 4;

  lbl_i.relabel(f);

  mln_assertion(lbl_i.nlabels() == 4);
  test_image(lbl_i, bboxes_2);
}
