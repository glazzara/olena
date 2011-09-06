// Copyright (C) 2009, 2011 EPITA Research and Development Laboratory
// (LRDE)
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

#include <mln/make/image2d.hh>
#include <mln/topo/skeleton/is_simple_point.hh>
#include <mln/make/image.hh>
#include <mln/make/dual_neighb.hh>

static const unsigned ref[] = { 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0 };

int main()
{
  using namespace mln;

  bool vals[][6] = { { 0, 1, 0, 0, 1, 1 },
                     { 0, 0, 0, 0, 1, 1 } };

  typedef image2d<bool> I;
  I ima = make::image(vals);

  typedef
    neighb<win::multiple_size<2u, window<dpoint<grid::square, short int> >,
           pw::value_<image2d<bool> > > >
    nbh_t;
  nbh_t nbh = make::dual_neighb(ima, c8(), c4());

  unsigned i = 0;
  topo::skeleton::is_simple_point<nbh_t> is_simple(nbh);

  // Test generic version.
  {
    mln_piter_(I) p(ima.domain());
    for_all(p)
      mln_assertion(ref[i++] == is_simple.check(ima, p));
  }

}
