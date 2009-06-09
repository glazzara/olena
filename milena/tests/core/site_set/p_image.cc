// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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
#include <mln/core/site_set/p_image.hh>

int main()
{
  using namespace mln;

  image2d<bool> ima(box2d(20,20));
  p_image<image2d<bool> > ps(ima);
  ps.insert(point2d(6, 9));
  ps.insert(point2d(4, 2));
  ps.insert(point2d(4, 2));
  ps.insert(point2d(5, 1));
  mln_assertion(ps.nsites() == 3);

  ps.remove(point2d(5, 1));
  ps.remove(point2d(5, 1));

  ps.remove(point2d(6, 9));
  ps.remove(point2d(4, 2));

  mln_assertion(ps.nsites() == 0);
  mln_assertion(ps.is_empty());

  mln_fwd_piter_(box2d) p(make::box2d(13,13,19,15));
  for_all(p)
  {
    ps.insert(p);
  }
  ps.clear();
  for_all(p)
  {
    mln_assertion(!ps.has(p));
  }

}
