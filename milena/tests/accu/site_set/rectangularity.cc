// Copyright (C) 2009, 2013 EPITA Research and Development Laboratory (LRDE)
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
#include <mln/core/alias/point2d.hh>
#include <mln/core/alias/box2d.hh>

#include <mln/accu/site_set/rectangularity.hh>
#include <mln/set/compute.hh>


int main()
{
  using namespace mln;

  {
    accu::site_set::rectangularity<point2d> accu;
    accu.take_as_init(point2d(0,0));
    accu.take(point2d(0,1));
    accu.take(point2d(1,0));
    accu.take(point2d(1,1));

    mln_assertion(accu.to_result() == 1);
  }

  {
    accu::site_set::rectangularity<point2d> accu;
    accu.take(point2d(0,0));
    accu.take(point2d(1,1));
    mln_assertion(accu.to_result() == 0.5f);
  }

  {
    box2d b(2,2);
    float r = mln::set::compute(accu::site_set::rectangularity<point2d>(), b);
    mln_assertion(r == 1.0f);
  }

  {
    accu::site_set::rectangularity<point2d> a;
    accu::site_set::rectangularity<point2d> b = exact(a);
    (void) b;
  }
}
