// Copyright (C) 2007 EPITA Research and Development Laboratory
//
// This file is part of the Milena Library.  This library is free
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

/*!
 *  \file   tests/abr.cc
 *
 *  \brief  test of mln::util::abr
 *
 */

#include <mln/util/abr.hh>
#include <mln/core/contract.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/set_p.hh>
#include <mln/value/int_u8.hh>
#include <mln/data/stretch.hh>
#include <mln/io/pgm/save.hh>
#include <vector>

namespace mln
{

  namespace util
  {

    template <typename T, typename I>
    void
    abr_to_image_rec(abr<T>& abr, Image<I>& output_, const mln_value(I) lvl)
    {
      I& output = exact(output_);

      mln_piter(T) p(abr.elt_);

      for_all(p)
	output(p) = lvl;

       typename std::vector< util::abr<T>* >::const_iterator it = abr.child_.begin();
      for (;
	   it != abr.child_.end();
	   ++it)
 	abr_to_image_rec((**it), output, lvl + 1);
    }


    template <typename T, typename I>
    void
    abr_to_image (abr<T>& abr, Image<I>& output_)
    {
      I& output = exact(output_);
      abr_to_image_rec(abr, output, 1);
    }

  } // end of namespace mln::util


} // end of namespace mln

int main (void)
{
  using namespace mln;
  using value::int_u8;

  typedef set_p<point2d > I;

  image2d<int_u8> output (300, 300);

  I s1;
  I s2;
  I s3;
  I s4;
  I s5;
  I s6;

  for (int i = 0; i < 100; ++i)
    for (int j = 0; j < 100; ++j)
      s1.insert(point2d(i, j));

  for (int i = 200; i < 300; ++i)
    for (int j = 0; j < 100; ++j)
      s2.insert(point2d(i, j));

  for (int i = 0; i < 100; ++i)
    for (int j = 0; j < 100; ++j)
      s3.insert(point2d(i, j));

  for (int i = 260; i < 290; ++i)
    for (int j = 0; j < 50; ++j)
      s4.insert(point2d(i, j));

  for (int i = 200; i < 210; ++i)
    for (int j = 0; j < 50; ++j)
      s5.insert(point2d(i, j));

  for (int i = 270; i < 280; ++i)
    for (int j = 50; j < 60; ++j)
      s6.insert(point2d(i, j));

  util::abr<I> abr(s1);
  abr.add_child(s2);
  abr.add_child(s3);

  util::abr<I>* abr2 = abr.search(s2);
  mln_assertion(abr2);
  abr2->add_child(s4);
  abr2->add_child(s5);
  util::abr<I>* abr3 = abr.search(s4);
  mln_assertion(abr3);
  abr3->add_child(s6);

  util::abr_to_image(abr, output);

  image2d<int_u8> out(output.domain());

  data::stretch (output, out);

  io::pgm::save(out, "out.pgm");

  std::cout << "out.pgm generate"
	    << std::endl;

//   abr3 = abr2->search(s1);
//   mln_assertion(!abr3);
}
