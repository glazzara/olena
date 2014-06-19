// Copyright (C) 2008, 2009, 2014 EPITA Research and Development
// Laboratory (LRDE).
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
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/site_set/p_array.hh>

#include <mln/data/sort_psites.hh>
#include <mln/data/compare.hh>
#include <mln/pw/value.hh>
#include <mln/debug/println.hh>

#include <mln/morpho/tree/compute_parent.hh>


static mln::image2d<mln::point2d> ref1()
{
  using namespace mln;

  image2d<point2d> ref1(make::box2d(3,3));

  ref1(point2d(0,0)) = point2d(0,0);
  ref1(point2d(0,1)) = point2d(0,0);
  ref1(point2d(0,2)) = point2d(0,0);
  ref1(point2d(1,1)) = point2d(0,0);
  ref1(point2d(2,2)) = point2d(2,2);

  return ref1;
}


static mln::image2d<mln::point2d> ref2()
{
  using namespace mln;

  image2d<point2d> ref2(make::box2d(3,3));

  ref2(point2d(0,0)) = point2d(0,1);
  ref2(point2d(0,1)) = point2d(0,2);
  ref2(point2d(0,2)) = point2d(0,2);
  ref2(point2d(1,0)) = point2d(0,0);
  ref2(point2d(1,1)) = point2d(0,1);
  ref2(point2d(1,2)) = point2d(0,1);
  ref2(point2d(2,0)) = point2d(0,0);
  ref2(point2d(2,1)) = point2d(0,0);
  ref2(point2d(2,2)) = point2d(0,2);

  return ref2;
}


int main()
{
  using namespace mln;

  {
    bool vals[] = { 1, 1, 1,
		    0, 1, 0,
		    0, 0, 1  };
    image2d<bool> ima = make::image2d(vals);
    typedef image_if<image2d<bool>, pw::value_<image2d<bool> > > sub_t;
    sub_t sub = ima | pw::value(ima);

    typedef image_if<image2d<point2d>, pw::value_<image2d<bool> > > par_t;
    par_t par = morpho::tree::compute_parent(sub, c4(), sub.domain());
    mln_assertion(par == (ref1() | sub.domain()));
  }

  {
    unsigned char vals[] = { 3, 2, 1,
			     3, 2, 3,
			     3, 4, 1 };
    image2d<unsigned char> ima = make::image2d(vals);

    typedef p_array<point2d> S;
    S s = data::sort_psites_increasing(ima);
    image2d<point2d> par = morpho::tree::compute_parent(ima, c4(), s);
    mln_assertion(par == ref2());
  }

}
