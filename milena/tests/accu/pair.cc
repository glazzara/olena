// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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
#include <mln/value/int_u8.hh>

#include <mln/accu/pair.hh>
#include <mln/accu/stat/max.hh>
#include <mln/accu/stat/mean.hh>

int main()
{
  using namespace mln;

  {
    accu::pair<accu::stat::mean<int>, accu::stat::max<int> > mean;

    mean.take(10);
    mean.take(9);
    mean.take(8);
    mean.take(7);
    mean.take(6);
    mean.take(5);
    mean.take(4);
    mean.take(3);
    mean.take(2);
    mean.take(1);
    mean.take(0);

    mln_assertion(mean.to_result().first == 5);
    mln_assertion(mean.to_result().second == 10);
  }
}
